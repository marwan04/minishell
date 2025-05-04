/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_redirection.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eaqrabaw <eaqrabaw@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/04 07:26:41 by eaqrabaw          #+#    #+#             */
/*   Updated: 2025/05/04 09:58:13 by eaqrabaw         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void handle_redir(t_ast *node, int std_fd)
{
    int fd;

    if (!node->file)
    {
        ft_putstr_fd("minishell: No file specified for redirection\n", STDERR_FILENO);
        exit(1);
    }
    fd = open_redir_file(node);
    if (fd < 0)
        exit(1);
    if (dup2(fd, std_fd) < 0)
        perror_and_exit("dup2");
    close(fd);
}

static void handle_heredoc(t_ast *node)
{
    if (node->heredoc_pipe[0] > 0)
    {
        if (dup2(node->heredoc_pipe[0], STDIN_FILENO) < 0)
            perror_and_exit("dup2");
        close(node->heredoc_pipe[0]);
        node->heredoc_pipe[0] = -1;
    }
}

void apply_redirections(t_ast *node)
{
    t_ast *cur = node;

    while (cur)
    {
        if (cur->type == NODE_HEREDOC)
            handle_heredoc(cur);
        else if (cur->type == NODE_REDIR_IN)
            handle_redir(cur, STDIN_FILENO);
        else if (cur->type == NODE_REDIR_OUT || cur->type == NODE_APPEND)
            handle_redir(cur, STDOUT_FILENO);
        cur = cur->right;
    }
}
