/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   herdoc_handler.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eaqrabaw <eaqrabaw@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/20 06:38:24 by eaqrabaw          #+#    #+#             */
/*   Updated: 2025/04/20 10:48:32 by eaqrabaw         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int read_failed(int  pipe_fd[2])
{
    close(pipe_fd[0]);
    close(pipe_fd[1]);
    return (HEREDOC_INTERRUPTED);
}

static void herdoc_success(t_ast *node, t_token *token)
{
    close(node->heredoc_pipe[1]);
    free(node->file);
    node->type = NODE_HEREDOC;
    delete_next_token(token);
}

int     handle_heredoc(t_ast *node, t_token *token)
{
    char *input = NULL;

    if (pipe(node->heredoc_pipe) == -1)
    {
        perror("Failed to create pipe for heredoc");
        return (HEREDOC_INTERRUPTED);
    }
    while (1)
    {
        input = readline("> ");
        if (!input)
            return (read_failed(node->heredoc_pipe));
        if (strcmp(input, node->file) == 0)
        {
            free(input);
            break;
        }
        write(node->heredoc_pipe[1], input, ft_strlen(input));
        write(node->heredoc_pipe[1], "\n", 1);
        free(input);
    }
    herdoc_success(node, token);
    return (0);
}
