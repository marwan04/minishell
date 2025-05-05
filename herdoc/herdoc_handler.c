/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   herdoc_handler.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: malrifai <malrifai@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/04 07:03:21 by eaqrabaw          #+#    #+#             */
/*   Updated: 2025/05/05 14:39:16 by malrifai         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	setup_heredoc_pipe_or_fail(t_ast *node, t_minishell *data)
{
	if (pipe(node->heredoc_pipe) < 0)
	{
		perror("pipe");
		data->last_exit_status = 1;
		return (1);
	}
	return (0);
}

static int	fork_heredoc_or_fail(pid_t *pid, t_ast *node, t_minishell *data)
{
	*pid = fork();
	if (*pid < 0)
	{
		perror("fork");
		close(node->heredoc_pipe[0]);
		close(node->heredoc_pipe[1]);
		data->last_exit_status = 1;
		return (1);
	}
	return (0);
}

static void	wait_heredoc_and_update(t_ast *node, t_minishell *data)
{
	int	status;

	close(node->heredoc_pipe[1]);
	if (waitpid(-1, &status, 0) < 0)
	{
		perror("waitpid");
		close(node->heredoc_pipe[0]);
		data->last_exit_status = 1;
		return ;
	}
	init_signals();
	if (WIFSIGNALED(status) && WTERMSIG(status) == SIGINT)
	{
		data->last_exit_status = 130;
		data->execution_aborted = 1;
	}
}

static void	process_heredoc_node(t_ast *node, t_minishell *data)
{
	pid_t	pid;

	if (!node || !node->heredoc_delim)
		return ;
	if (setup_heredoc_pipe_or_fail(node, data))
		return ;
	if (fork_heredoc_or_fail(&pid, node, data))
		return ;
	ign_sig();
	if (pid == 0)
		heredoc_child(node, data);
	wait_heredoc_and_update(node, data);
}

void	collect_heredocs(t_ast *node, t_minishell *data)
{
	if (!node)
		return ;
	if (node->type == NODE_HEREDOC)
		process_heredoc_node(node, data);
	if (node->left)
		collect_heredocs(node->left, data);
	if (node->right)
		collect_heredocs(node->right, data);
}
