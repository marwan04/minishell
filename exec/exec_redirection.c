/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_redirection.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: malrifai <malrifai@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/11 19:02:30 by malrifai          #+#    #+#             */
/*   Updated: 2025/04/12 16:54:02 by malrifai         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int handle_redirection_node(t_ast *node, int prev_fd, t_minishell *data)
{
	int fd;
	pid_t pid;
	int status;

	if (!node || !node->file)
		return 1;
	if (node->type == NODE_REDIR_IN)
		fd = open(node->file, O_RDONLY);
	else if (node->type == NODE_REDIR_OUT)
		fd = open(node->file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	else
		fd = open(node->file, O_WRONLY | O_CREAT | O_APPEND, 0644);
	if (fd < 0)
	{
		perror(node->file);
		return 1;
	}
	pid = fork();
	if (pid == 0)
	{
		if (node->type == NODE_REDIR_IN)
			dup2(fd, STDIN_FILENO);
		else
			dup2(fd, STDOUT_FILENO);
		close(fd);
		exec_ast(node->left, prev_fd, data);
		ft_free(data, 1, "");
	}
	close(fd);
	if (prev_fd != -1)
		close(prev_fd);
	waitpid(pid, &status, 0);
	data->last_exit_status = WEXITSTATUS(status);
	return data->last_exit_status;
}
