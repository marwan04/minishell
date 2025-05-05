/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipeline_fork.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: malrifai <malrifai@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/05 13:58:47 by malrifai          #+#    #+#             */
/*   Updated: 2025/05/05 14:09:48 by malrifai         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	setup_child_args(t_child_args *c, t_fork_args *f, int i)
{
	c->stages = f->stages;
	c->i = i;
	c->pipe_count = f->pipe_count;
	c->pipes = f->pipes;
	c->prev_fd = f->prev_fd;
	c->data = f->data;
}

static int	handle_fork_error_pipe(void)
{
	perror("fork");
	return (1);
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

int	fork_and_exec(t_fork_args *f)
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
