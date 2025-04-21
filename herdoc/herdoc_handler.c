/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   herdoc_handler.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eaqrabaw <eaqrabaw@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/20 06:38:24 by eaqrabaw          #+#    #+#             */
/*   Updated: 2025/04/21 23:28:30 by eaqrabaw         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int herdoc_failed(int pipefd[2])
{
    close(pipefd[0]);
    close(pipefd[1]);
    return (HEREDOC_INTERRUPTED);
}

static int process_heredoc(t_ast *node)
{
    char *line;

    if (pipe(node->heredoc_pipe) == -1)
        return (herdoc_failed(node->heredoc_pipe));
    while ((line = readline("> ")))
    {
        if (strcmp(line, node->file) == 0)
        {
            free(line);
            break;
        }
        write(node->heredoc_pipe[1], line, strlen(line));
        write(node->heredoc_pipe[1], "\n", 1);
        free(line);
    }
    if (!line)
        return (herdoc_failed(node->heredoc_pipe));
    close(node->heredoc_pipe[1]);
    return (0);
}

void collect_heredocs(t_ast *node)
{
    if (!node)
        return;
        
    collect_heredocs(node->left);
    collect_heredocs(node->right);
    if (node->type == NODE_HEREDOC)
    {
        if (process_heredoc(node) != 0)
        {
            return;
        }
    }
}
