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
	int	stdin_fd;
	int	stdout_fd;

	pid = fork();
	if (pid == 0)
	{
		signal(SIGINT, SIG_DFL);
		signal(SIGQUIT, SIG_DFL);
		
		// Setup input
		if (prev_fd != -1)
		{
			stdin_fd = dup(prev_fd);
			if (stdin_fd == -1)
			{
				perror("dup");
				exit(1);
			}
			dup2(stdin_fd, STDIN_FILENO);
			close(prev_fd);
		}
		
		// Setup heredoc input
		if (node->heredoc_pipe[0] >= 0)
		{
			stdin_fd = dup(node->heredoc_pipe[0]);
			if (stdin_fd == -1)
			{
				perror("dup");
				exit(1);
			}
			dup2(stdin_fd, STDIN_FILENO);
			close(node->heredoc_pipe[0]);
			close(node->heredoc_pipe[1]);
		}
		
		// Setup output
		if (prev_fd != -1)
		{
			stdout_fd = dup(prev_fd);
			if (stdout_fd == -1)
			{
				perror("dup");
				exit(1);
			}
			dup2(stdout_fd, STDOUT_FILENO);
		}
		
		// Execute command
		cmd = node->left;
		while (cmd && cmd->type == NODE_HEREDOC)
			cmd = cmd->left;
		if (exec_ast(cmd, -1, data) == -1)
		{
			perror("exec");
			exit(1);
		}
	}
	return (pid);
}

int	handle_heredoc_node(t_ast *node, int prev_fd, t_minishell *data)
{
	pid_t	pid;
	int	status;

	if (!node)
		return (1);

	// Create heredoc pipe
	if (pipe(node->heredoc_pipe) == -1)
	{
		perror("pipe");
		return (1);
	}

	pid = fork_and_exec_heredoc(node, prev_fd, data);
	if (pid < 0)
	{
		perror("fork");
		close(node->heredoc_pipe[0]);
		close(node->heredoc_pipe[1]);
		return (1);
	}

	// Parent process
	if (prev_fd != -1)
		close(prev_fd);
	
	// Write to heredoc
	close(node->heredoc_pipe[0]); // Close read end
	write_heredoc(node->heredoc_delim, node->heredoc_pipe[1]);
	close(node->heredoc_pipe[1]); // Close write end

	// Wait for child
	waitpid(pid, &status, 0);
	data->last_exit_status = WEXITSTATUS(status);
	return (data->last_exit_status);
}
