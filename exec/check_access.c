/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_access.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: malrifai <malrifai@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/01 00:39:28 by eaqrabaw          #+#    #+#             */
/*   Updated: 2025/05/05 00:29:11 by malrifai         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	open_redir_in_file(t_ast *node)
{
	int	fd;

	if (access(node->file, F_OK) == -1)
	{
		perror("File does not exist");
		return (-1);
	}
	if (access(node->file, R_OK) == -1)
	{
		perror("No read permission");
		return (-1);
	}
	fd = open(node->file, O_RDONLY);
	if (fd < 0)
	{
		perror(node->file);
		return (-1);
	}
	return (fd);
}

int	open_redir_out_file(t_ast *node)
{
	int	fd;

	if (access(node->file, F_OK) != -1)
	{
		if (access(node->file, W_OK) == -1)
		{
			perror("No write permission");
			return (-1);
		}
	}
	fd = open(node->file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (fd < 0)
	{
		perror(node->file);
		return (-1);
	}
	return (fd);
}

int	open_redir_append_file(t_ast *node)
{
	int	fd;

	if (access(node->file, F_OK) != -1)
	{
		if (access(node->file, W_OK) == -1)
		{
			perror("No write permission");
			return (-1);
		}
	}
	fd = open(node->file, O_WRONLY | O_CREAT | O_APPEND, 0644);
	if (fd < 0)
	{
		perror(node->file);
		return (-1);
	}
	return (fd);
}

int	open_redir_file(t_ast *node)
{
	if (node->type == NODE_REDIR_IN)
		return (open_redir_in_file(node));
	else if (node->type == NODE_REDIR_OUT)
		return (open_redir_out_file(node));
	else
		return (open_redir_append_file(node));
}
