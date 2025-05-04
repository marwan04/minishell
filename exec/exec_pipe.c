/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_pipe.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eaqrabaw <eaqrabaw@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/04 12:00:00 by student           #+#    #+#             */
/*   Updated: 2025/05/04 11:00:40 by eaqrabaw         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "minishell.h"

static void	free_pipeline_resources(int (*pipes)[2], int pipe_cnt, pid_t *pids)
{
	int	i;

	if (pipes)
	{
		i = 0;
		while (i < pipe_cnt)
		{
			close(pipes[i][0]);
			close(pipes[i][1]);
			i++;
		}
		free(pipes);
	}
	if (pids)
		free(pids);
}

static t_ast	*find_cmd_node(t_ast *node)
{
	while (node && (node->type == NODE_HEREDOC || node->type == NODE_REDIR_IN
			|| node->type == NODE_REDIR_OUT || node->type == NODE_APPEND))
		node = node->left;
	return (node);
}

static int	setup_redirections(t_ast *stage, int *used_in, int *used_out)
{
	t_ast	*r;

	if (apply_redirections(stage) < 0)
		return (-1);
	*used_in = 0;
	*used_out = 0;
	r = stage;
	while (r && (r->type == NODE_HEREDOC || r->type == NODE_REDIR_IN
			|| r->type == NODE_REDIR_OUT || r->type == NODE_APPEND))
	{
		if (r->type == NODE_HEREDOC || r->type == NODE_REDIR_IN)
			*used_in = 1;
		if (r->type == NODE_REDIR_OUT || r->type == NODE_APPEND)
			*used_out = 1;
		r = r->right;
	}
	return (0);
}

static void	setup_child_pipes(int i, int n_stages, int (*pipes)[2],
				int used_in, int used_out)
{
	int	j;

	j = 0;
	while (j < n_stages - 1)
	{
		if (j == i - 1 && !used_in)
			dup2(pipes[j][0], STDIN_FILENO);
		if (j == i && !used_out)
			dup2(pipes[j][1], STDOUT_FILENO);
		close(pipes[j][0]);
		close(pipes[j][1]);
		j++;
	}
}

static void	exec_child_cmd(t_ast *cmd, t_minishell *data)
{
	char	*path;
	int		ret;

	if (!cmd || !cmd->args || !cmd->args[0])
		exit(0);
	if (is_builtin(cmd->args[0]))
	{
		ret = 0;
		execute_builtin_cmds(cmd, &ret, &data->env);
		exit(ret);
	}
	path = ft_get_path(cmd->args[0], &data->env);
	if (!path)
	{
        ft_putstr_fd(cmd->args[0], 2);
        ft_putstr_fd(": command not found\n", 2);
		exit(127);
	}
	execve(path, cmd->args, envp_to_array(data->env));
	perror("execve");
	exit(126);
}

static void	handle_child_process(t_ast **stages, int i, int n_stages,
				t_pipe_data p_data)
{
	int		used_in;
	int		used_out;
	t_ast	*cmd;

	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
	if (setup_redirections(stages[i], &used_in, &used_out) < 0)
		exit(1);
	setup_child_pipes(i, n_stages, p_data.pipes, used_in, used_out);
	if (p_data.prev_fd > STDERR_FILENO)
		close(p_data.prev_fd);
	cmd = find_cmd_node(stages[i]);
	exec_child_cmd(cmd, p_data.data);
}

static int	create_pipes(int (*pipes)[2], int pipe_count)
{
	int	i;

	i = 0;
	while (i < pipe_count)
	{
		if (pipe(pipes[i]) < 0)
		{
			perror("pipe");
			free_pipeline_resources(pipes, i, NULL);
			return (-1);
		}
		i++;
	}
	return (0);
}

static int	create_child_processes(t_ast **stages, int n_stages,
				t_pipe_data p_data, pid_t *pids)
{
	int		i;
	pid_t	pid;

	i = 0;
	while (i < n_stages)
	{
		pid = fork();
		if (pid < 0)
		{
			perror("fork");
			free_pipeline_resources(p_data.pipes, n_stages - 1, pids);
			return (-1);
		}
		if (pid == 0)
			handle_child_process(stages, i, n_stages, p_data);
		pids[i] = pid;
		i++;
	}
	return (0);
}

static int	wait_for_children(pid_t *pids, int n_stages)
{
	int	status;
	int	last_status;
	int	i;

	i = 0;
	last_status = 0;
	while (i < n_stages)
	{
		if (waitpid(pids[i], &status, 0) < 0)
		{
			perror("waitpid");
			last_status = 1;
		}
		else
			last_status = WEXITSTATUS(status);
		i++;
	}
	return (last_status);
}

static t_pipe_data	init_pipe_data(int prev_fd, t_minishell *data,
					int (*pipes)[2])
{
	t_pipe_data	p_data;

	p_data.prev_fd = prev_fd;
	p_data.data = data;
	p_data.pipes = pipes;
	return (p_data);
}
static int cleanup_pipeline(int prev_fd, int **pipes, int pipe_count)
{
    if (prev_fd > STDERR_FILENO)
        close(prev_fd);

    free_pipeline_resources(pipes, pipe_count, NULL);

    int last_status = wait_for_children(pids, n_stages);
    
    close_heredoc_pipes_in_stages(stages, n_stages);
    
    free(pids);

    return last_status;
}
int exec_pipeline(t_ast **stages, int n_stages, int prev_fd, t_minishell *data)
{
    int (*pipes)[2];
    pid_t *pids;
    int last_status;
    int pipe_count;
    t_pipe_data p_data;

    if (n_stages <= 1)
        return exec_cmd_node(stages[0], prev_fd, data);

    pipe_count = n_stages - 1;
    pipes = malloc(pipe_count * sizeof(*pipes));
    pids = malloc(n_stages * sizeof(*pids));
    
    if (!pipes || !pids)
    {
        perror("malloc");
        free_pipeline_resources(pipes, 0, pids);
        return 1;
    }

    if (create_pipes(pipes, pipe_count) < 0)
        return 1;

    p_data = init_pipe_data(prev_fd, data, pipes);

    if (create_child_processes(stages, n_stages, p_data, pids) < 0)
        return 1;

    last_status = cleanup_pipeline(prev_fd, pipes, pipe_count, pids, n_stages, stages);

    return last_status;
}

