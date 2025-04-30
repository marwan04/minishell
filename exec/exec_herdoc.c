/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_herdoc.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: malrifai <malrifai@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/22 00:29:25 by eaqrabaw          #+#    #+#             */
/*   Updated: 2025/04/30 17:52:42 by malrifai         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static pid_t	fork_and_exec_heredoc(t_ast *node,
	int prev_fd, t_minishell *data)
{
	pid_t	pid;
	t_ast	*cmd;

	pid = fork();
	if (pid == 0)
	{
		signal(SIGINT, SIG_DFL);
		signal(SIGQUIT, SIG_DFL);
		if (prev_fd != -1)
		{
			dup2(prev_fd, STDOUT_FILENO);
			close(prev_fd);
		}
		dup2(node->heredoc_pipe[0], STDIN_FILENO);
		close(node->heredoc_pipe[0]);
		cmd = node->left;
		while (cmd && cmd->type == NODE_HEREDOC)
			cmd = cmd->left;
		exec_ast(cmd, -1, data);
		ft_free(data, data->last_exit_status, "");
		exit(data->last_exit_status);
	}
	return (pid);
}

int	handle_heredoc_node(t_ast *node, int prev_fd, t_minishell *data)
{
	pid_t	pid;
	int		status;

	if (!node)
		return (1);
	pid = fork_and_exec_heredoc(node, prev_fd, data);
	if (pid < 0)
	{
		perror("fork");
		return (1);
	}
	if (prev_fd != -1)
		close(prev_fd);
	close(node->heredoc_pipe[0]);
	waitpid(pid, &status, 0);
	data->last_exit_status = WEXITSTATUS(status);
	return (data->last_exit_status);
}
