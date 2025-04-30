/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_redirection.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eaqrabaw <eaqrabaw@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/11 19:02:30 by malrifai          #+#    #+#             */
/*   Updated: 2025/05/01 00:48:41 by eaqrabaw         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

pid_t	fork_and_exec_redir(t_ast *node, int fd, int prev_fd, t_minishell *data)
{
	pid_t	pid;

	pid = fork();
	if (pid == 0)
	{
		signal(SIGINT, SIG_DFL);
		signal(SIGQUIT, SIG_DFL);
		if (node->type == NODE_REDIR_IN)
			dup2(fd, STDIN_FILENO);
		else
			dup2(fd, STDOUT_FILENO);
		close(fd);
		exec_ast(node->left, prev_fd, data);
		ft_free(data, data->last_exit_status, "");
		exit(data->last_exit_status);
	}
	return (pid);
}

int	handle_redirection_node(t_ast *node, int prev_fd, t_minishell *data)
{
	int		fd;
	int		status;
	pid_t	pid;

	if (!node || !node->file)
		return (1);
	fd = open_redir_file(node);
	if (fd < 0)
	{
		data->last_exit_status = 1;
		return (data->last_exit_status);
	}
	pid = fork_and_exec_redir(node, fd, prev_fd, data);
	close(fd);
	if (prev_fd != -1)
		close(prev_fd);
	waitpid(pid, &status, 0);
	data->last_exit_status = WEXITSTATUS(status);
	return (data->last_exit_status);
}
