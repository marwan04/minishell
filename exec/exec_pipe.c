/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_pipe.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: malrifai <malrifai@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/04 12:00:00 by student           #+#    #+#             */
/*   Updated: 2025/05/05 13:31:07 by malrifai         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	create_pipes(int pipe_count, int (*pipes)[2])
{
	int	j;
	int	i;

	i = 0;
	while (i < pipe_count)
	{
		if (pipe(pipes[i]) < 0)
		{
			perror("pipe");
			j = 0;
			while (j < i)
			{
				close(pipes[j][0]);
				close(pipes[j][1]);
				j++;
			}
			return (1);
		}
		i++;
	}
	return (0);
}

static void	handle_pipe_redirects(t_redirect_args *a, int used_in, int used_out)
{
	int	j;
	int	rd;
	int	wr;

	j = 0;
	while (j < a->pipe_count)
	{
		rd = a->pipes[j][0];
		wr = a->pipes[j][1];
		if (j == a->index - 1 && !used_in)
			dup2(rd, STDIN_FILENO);
		if (j == a->index && !used_out)
			dup2(wr, STDOUT_FILENO);
		close(rd);
		close(wr);
		j++;
	}
}

static void	detect_redirect_usage(t_ast *r, int *in, int *out)
{
	*in = 0;
	*out = 0;
	while (r && (r->type == NODE_HEREDOC || r->type == NODE_REDIR_IN
			|| r->type == NODE_REDIR_OUT || r->type == NODE_APPEND))
	{
		if (r->type == NODE_HEREDOC || r->type == NODE_REDIR_IN)
			*in = 1;
		if (r->type == NODE_REDIR_OUT || r->type == NODE_APPEND)
			*out = 1;
		r = r->right;
	}
}

static void	exec_setup_redirects(t_redirect_args *a)
{
	int	used_in;
	int	used_out;

	detect_redirect_usage(a->stage, &used_in, &used_out);
	handle_pipe_redirects(a, used_in, used_out);
}

static void	handle_external_command(t_ast *cmd, t_minishell *data)
{
	char	*path;
	char	**envp_arr;

	path = ft_get_path(cmd->args[0], &data->env);
	if (!path)
	{
		ft_putstr_fd(cmd->args[0], 2);
		ft_putstr_fd(": command not found\n", 2);
		ft_free(data, 127, NULL);
	}
	envp_arr = envp_to_array(data->env);
	execve(path, cmd->args, envp_arr);
	perror("execve");
	free(path);
	ft_free_double_list(envp_arr);
	ft_free(data, 126, NULL);
}

static void	exec_handle_command(t_ast *cmd, t_minishell *data)
{
	int	ret;

	ret = 0;
	if (is_builtin(cmd->args[0]))
	{
		execute_builtin_cmds(cmd, &ret, &data->env);
		ft_free(data, ret, NULL);
	}
	else
		handle_external_command(cmd, data);
}

static t_ast	*exec_extract_command(t_ast *stage)
{
	t_ast	*cmd;

	cmd = stage;
	while (cmd && (cmd->type == NODE_HEREDOC || cmd->type == NODE_REDIR_IN
			|| cmd->type == NODE_REDIR_OUT || cmd->type == NODE_APPEND))
		cmd = cmd->left;
	return (cmd);
}

static void	exec_child(t_child_args *c)
{
	t_ast			*cmd;
	t_redirect_args	r_args;

	def_sig();
	apply_redirections(c->stages[c->i]);
	close_heredoc_pipes_in_stages(c->stages, c->pipe_count + 1);
	r_args.stage = c->stages[c->i];
	r_args.pipe_count = c->pipe_count;
	r_args.index = c->i;
	r_args.pipes = c->pipes;
	exec_setup_redirects(&r_args);
	if (c->prev_fd > STDERR_FILENO)
		close(c->prev_fd);
	cmd = exec_extract_command(c->stages[c->i]);
	if (cmd && cmd->args && cmd->args[0])
		exec_handle_command(cmd, c->data);
	ft_free(c->data, 1, NULL);
}

static int	handle_fork_error_pipe(void)
{
	perror("fork");
	return (1);
}

static void	setup_child_args(t_child_args *c, t_fork_args *f, int i)
{
	c->stages = f->stages;
	c->i = i;
	c->pipe_count = f->pipe_count;
	c->pipes = f->pipes;
	c->prev_fd = f->prev_fd;
	c->data = f->data;
}

static int	fork_and_exec(t_fork_args *f)
{
	int				i;
	t_child_args	c;

	i = 0;
	while (i < f->n_stages)
	{
		ign_sig();
		f->pids[i] = fork();
		if (f->pids[i] < 0)
			return (handle_fork_error_pipe());
		if (f->pids[i] == 0)
		{
			setup_child_args(&c, f, i);
			exec_child(&c);
		}
		i++;
	}
	return (0);
}

static void	close_all_pipes(int pipe_count, int (*pipes)[2])
{
	int	i;

	i = 0;
	while (i < pipe_count)
	{
		close(pipes[i][0]);
		close(pipes[i][1]);
		i++;
	}
}

static int	wait_for_all(pid_t *pids, int n_stages)
{
	int	i;
	int	status;
	int	last_status;

	i = 0;
	last_status = 0;
	while (i < n_stages)
	{
		if (waitpid(pids[i], &status, 0) < 0)
			perror("waitpid");
		else
			last_status = WEXITSTATUS(status);
		i++;
	}
	return (last_status);
}

static int	handle_pipeline_allocation(int pipe_count, int n_stages,
			int (**pipes)[2], pid_t **pids)
{
	*pipes = malloc(pipe_count * sizeof(**pipes));
	*pids = malloc(n_stages * sizeof(**pids));
	if (!*pipes || !*pids)
	{
		perror("malloc");
		free(*pipes);
		free(*pids);
		return (1);
	}
	return (0);
}

int	exec_pipeline_helper(t_exec_pipeline_args *a,
						int pipe_count, int (**pipes)[2], pid_t **pids)
{
	t_fork_args	f;

	if (create_pipes(pipe_count, *pipes))
	{
		free(*pipes);
		free(*pids);
		return (1);
	}
	f.stages = a->stages;
	f.n_stages = a->n_stages;
	f.pipe_count = pipe_count;
	f.prev_fd = a->prev_fd;
	f.pipes = *pipes;
	f.pids = *pids;
	f.data = a->data;
	if (fork_and_exec(&f))
	{
		free(*pipes);
		free(*pids);
		return (1);
	}
	return (0);
}

int	exec_pipeline_main(t_exec_pipeline_args *a, int *last_status)
{
	int			(*pipes)[2];
	pid_t		*pids;
	int			pipe_count;

	pipes = NULL;
	pids = NULL;
	pipe_count = a->n_stages - 1;
	if (handle_pipeline_allocation(pipe_count, a->n_stages, &pipes, &pids))
		return (1);
	if (exec_pipeline_helper(a, pipe_count, &pipes, &pids))
		return (1);
	if (a->prev_fd > STDERR_FILENO)
		close(a->prev_fd);
	close_all_pipes(pipe_count, pipes);
	close_heredoc_pipes_in_stages(a->stages, a->n_stages);
	*last_status = wait_for_all(pids, a->n_stages);
	init_signals();
	free(pipes);
	free(pids);
	return (0);
}

int	exec_pipeline(t_ast **stages, int n_stages, int prev_fd, t_minishell *data)
{
	t_exec_pipeline_args	args;
	int						last_status;

	args.stages = stages;
	args.n_stages = n_stages;
	args.prev_fd = prev_fd;
	args.data = data;
	last_status = 0;
	if (n_stages <= 1)
		return (exec_cmd_node(stages[0], prev_fd, data));
	if (exec_pipeline_main(&args, &last_status))
		return (1);
	return (last_status);
}
