/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   herdoc_handler.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eaqrabaw <eaqrabaw@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/20 06:38:24 by eaqrabaw          #+#    #+#             */
/*   Updated: 2025/05/01 00:45:52 by eaqrabaw         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	print_heredoc_eof_warning(char *delimiter)
{
	write(2, "minishell: warning: heredoc delimited by EOF (wanted '", 55);
	write(2, delimiter, ft_strlen(delimiter));
	write(2, "')\n", 3);
}

static int	handle_heredoc_end(t_ast *node, t_minishell *data, char *line)
{
	if (!line && g_sig_int)
	{
		close(node->heredoc_pipe[0]);
		close(node->heredoc_pipe[1]);
		data->last_exit_status = 130;
		return (HEREDOC_INTERRUPTED_SIG);
	}
	if (!line)
	{
		close(node->heredoc_pipe[0]);
		close(node->heredoc_pipe[1]);
		print_heredoc_eof_warning(node->file);
		return (HEREDOC_EOF);
	}
	close(node->heredoc_pipe[1]);
	return (HEREDOC_SUCCESS);
}

int process_heredoc(t_ast *node, t_minishell *data)
{
    char *line;

    if (pipe(node->heredoc_pipe) == -1)
        return (HEREDOC_INTERRUPTED_SIG);
    g_sig_int = 0;
    printf("herdoc del is : %s\n", node->file);
    line = readline("> ");
    while (line)
    {
        if (ft_strcmp(line, node->file) == 0)
        {
            free(line);
            break ;
        }
        if (node->heredoc_expand)
            line = expand_line(line, data);

        write(node->heredoc_pipe[1], line, ft_strlen(line));
        write(node->heredoc_pipe[1], "\n", 1);
        free(line);
        line = readline("> ");
    }
    close(node->heredoc_pipe[1]);
    return (handle_heredoc_end(node, data, line));
}

void	collect_heredocs(t_ast *node, t_minishell *data)
{
	if (!node)
		return ;
	collect_heredocs(node->left, data);
	collect_heredocs(node->right, data);
	if (node->type == NODE_HEREDOC)
	{
		if (process_heredoc(node, data) == HEREDOC_INTERRUPTED_SIG)
		{
			data->execution_aborted = 1;
			return ;
		}
	}
}
