/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   herdoc_handler.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eaqrabaw <eaqrabaw@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/20 06:38:24 by eaqrabaw          #+#    #+#             */
/*   Updated: 2025/04/27 21:27:54 by eaqrabaw         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	print_heredoc_eof_warning(char *delimiter)
{
	write(2, "minishell: warning: heredoc delimited by EOF (wanted '", 55);
	write(2, delimiter, ft_strlen(delimiter));
	write(2, "')\n", 3);
}

static int	heredoc_failed(int pipefd[2])
{
	close(pipefd[0]);
	close(pipefd[1]);
	return (HEREDOC_INTERRUPTED);
}

static int	process_heredoc(t_ast *node, t_minishell *data)
{
	char	*line;

	if (pipe(node->heredoc_pipe) == -1)
		return (heredoc_failed(node->heredoc_pipe));
	while ((line = readline("> ")))
	{
		if (ft_strcmp(line, node->file) == 0)
		{
			free(line);
			break;
		}
		printf("The herdoc expand var = %d\n", node->heredoc_expand);
		if (node->heredoc_expand == 1)
			line = expand_line(line, data);
		write(node->heredoc_pipe[1], line, ft_strlen(line));
		write(node->heredoc_pipe[1], "\n", 1);
		free(line);
	}
	if (!line)
	{
        print_heredoc_eof_warning(node->file);
		return (heredoc_failed(node->heredoc_pipe));
	}
	close(node->heredoc_pipe[1]);
	free(node->file);
	node->file = NULL;
	return (0);
}

void	collect_heredocs(t_ast *node, t_minishell *data)
{
	if (!node)
		return;
	collect_heredocs(node->left, data);
	collect_heredocs(node->right, data);
	if (node->type == NODE_HEREDOC)
	{
		if (process_heredoc(node, data) != 0)
		{
			data->last_exit_status = 130;
			return ;
		}
	}
}