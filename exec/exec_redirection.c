/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_redirection.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: malrifai <malrifai@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/04 07:26:41 by eaqrabaw          #+#    #+#             */
/*   Updated: 2025/05/05 10:58:40 by malrifai         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	handle_redir(t_ast *node, int std_fd)
{
	int	fd;

	if (!node->file)
	{
		ft_putstr_fd
			("minishell: No file specified for redirection\n", STDERR_FILENO);
		return (-1);
	}
	fd = open_redir_file(node);
	if (fd < 0)
		return (-1);
	if (dup2(fd, std_fd) < 0)
	{
		perror("dup2");
		close(fd);
		return (-1);
	}
	close(fd);
	return (0);
}

static int	handle_heredoc(t_ast *node)
{
	if (node->heredoc_pipe[0] > 0)
	{
		if (dup2(node->heredoc_pipe[0], STDIN_FILENO) < 0)
		{
			perror("dup2");
			close(node->heredoc_pipe[0]);
			return (-1);
		}
		close(node->heredoc_pipe[0]);
		node->heredoc_pipe[0] = -1;
	}
	return (0);
}

int	apply_redirections(t_ast *node)
{
	t_ast	*cur;
	int		fd;

	cur = node;
	while (cur)
	{
		if (cur->type == NODE_HEREDOC)
		{
			if (handle_heredoc(cur) < 0)
				return (-1);
		}
		else if (cur->type == NODE_REDIR_IN
			|| cur->type == NODE_REDIR_OUT || cur->type == NODE_APPEND)
		{
			if (cur->type == NODE_REDIR_IN)
				fd = STDIN_FILENO;
			else
				fd = STDOUT_FILENO;
			if (handle_redir(cur, fd) < 0)
				return (-1);
		}
		cur = cur->right;
	}
	return (0);
}
