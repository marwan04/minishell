/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   herdoc_handler.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alrfa3i <alrfa3i@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/20 06:38:24 by eaqrabaw          #+#    #+#             */
/*   Updated: 2025/04/30 11:42:14 by alrfa3i          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	print_heredoc_eof_warning(char *delimiter)
{
	write(2, "minishell: warning: heredoc delimited by EOF (wanted '", 55);
	write(2, delimiter, ft_strlen(delimiter));
	write(2, "')\n", 3);
}

// static int	heredoc_failed(int pipefd[2])
// {
// 	close(pipefd[0]);
// 	close(pipefd[1]);
// 	return (HEREDOC_INTERRUPTED_SIG);
// }

int process_heredoc(t_ast *node, t_minishell *data)
{
	char *line;

	if (pipe(node->heredoc_pipe) == -1)
		return HEREDOC_INTERRUPTED_SIG;

	g_sig_int = 0;
	while ((line = readline("> ")))
	{
		if (ft_strcmp(line, node->file) == 0)
		{
			free(line);
			break;
		}
		if (node->heredoc_expand)
			line = expand_line(line, data);
		write(node->heredoc_pipe[1], line, ft_strlen(line));
		write(node->heredoc_pipe[1], "\n", 1);
		free(line);
	}

	// ❗ Null line = EOF or SIGINT
	if (!line)
	{
		if (g_sig_int)
		{
			// Ctrl+C behavior: abort heredoc completely
			close(node->heredoc_pipe[0]);
			close(node->heredoc_pipe[1]);
			data->last_exit_status = 130;
			return (HEREDOC_INTERRUPTED_SIG);
		}
		else
		{
			close(node->heredoc_pipe[0]);
			close(node->heredoc_pipe[1]);
			// Ctrl+D behavior: show warning, continue execution
			print_heredoc_eof_warning(node->file);
			close(node->heredoc_pipe[1]);
			return (HEREDOC_EOF);
		}
	}
	close(node->heredoc_pipe[0]);
	close(node->heredoc_pipe[1]);
	return (HEREDOC_SUCCESS);
}


void collect_heredocs(t_ast *node, t_minishell *data)
{
    if (!node)
        return;
    
    collect_heredocs(node->left, data);
    collect_heredocs(node->right, data);

    if (node->type == NODE_HEREDOC)
    {
        if (process_heredoc(node, data) == HEREDOC_INTERRUPTED_SIG)
        {
            data->execution_aborted = 1;
            return;
        }
    }
}
