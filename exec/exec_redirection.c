/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_redirection.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eaqrabaw <eaqrabaw@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/04 07:26:41 by eaqrabaw          #+#    #+#             */
/*   Updated: 2025/05/04 07:31:09 by eaqrabaw         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void        apply_redirections(t_ast *node)
{
    t_ast *cur;
    int    fd;

    if (!node)
        return;

    /* First, apply the redirection represented by this node */
    if (node->type == NODE_HEREDOC)
    {
        if (node->heredoc_pipe[0] > 0) {
            if (dup2(node->heredoc_pipe[0], STDIN_FILENO) < 0)
                perror_and_exit("dup2");
            close(node->heredoc_pipe[0]);
            node->heredoc_pipe[0] = -1; // Mark as used
        }
    }
    else if (node->type == NODE_REDIR_IN)
    {
        if (!node->file) {
            fprintf(stderr, "minishell: No file specified for input redirection\n");
            exit(1);
        }
        
        fd = open(node->file, O_RDONLY);
        if (fd < 0) {
            perror(node->file);
            exit(1);
        }
        if (dup2(fd, STDIN_FILENO) < 0)
            perror_and_exit("dup2");
        close(fd);
    }
    else if (node->type == NODE_REDIR_OUT)
    {
        if (!node->file) {
            fprintf(stderr, "minishell: No file specified for output redirection\n");
            exit(1);
        }
        
        fd = open(node->file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
        if (fd < 0) {
            perror(node->file);
            exit(1);
        }
        if (dup2(fd, STDOUT_FILENO) < 0)
            perror_and_exit("dup2");
        close(fd);
    }
    else if (node->type == NODE_APPEND)
    {
        if (!node->file) {
            fprintf(stderr, "minishell: No file specified for append redirection\n");
            exit(1);
        }
        
        fd = open(node->file, O_WRONLY | O_CREAT | O_APPEND, 0644);
        if (fd < 0) {
            perror(node->file);
            exit(1);
        }
        if (dup2(fd, STDOUT_FILENO) < 0)
            perror_and_exit("dup2");
        close(fd);
    }

    /* Then, apply any additional redirections chained on the right */
    cur = node->right;
    while (cur)
    {
        if (cur->type == NODE_HEREDOC)
        {
            if (cur->heredoc_pipe[0] > 0) {
                if (dup2(cur->heredoc_pipe[0], STDIN_FILENO) < 0)
                    perror_and_exit("dup2");
                close(cur->heredoc_pipe[0]);
                cur->heredoc_pipe[0] = -1; // Mark as used
            }
        }
        else if (cur->type == NODE_REDIR_IN)
        {
            if (!cur->file) {
                fprintf(stderr, "minishell: No file specified for input redirection\n");
                exit(1);
            }
            
            fd = open(cur->file, O_RDONLY);
            if (fd < 0) {
                perror(cur->file);
                exit(1);
            }
            if (dup2(fd, STDIN_FILENO) < 0)
                perror_and_exit("dup2");
            close(fd);
        }
        else if (cur->type == NODE_REDIR_OUT)
        {
            if (!cur->file) {
                fprintf(stderr, "minishell: No file specified for output redirection\n");
                exit(1);
            }
            
            fd = open(cur->file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
            if (fd < 0) {
                perror(cur->file);
                exit(1);
            }
            if (dup2(fd, STDOUT_FILENO) < 0)
                perror_and_exit("dup2");
            close(fd);
        }
        else if (cur->type == NODE_APPEND)
        {
            if (!cur->file) {
                fprintf(stderr, "minishell: No file specified for append redirection\n");
                exit(1);
            }
            
            fd = open(cur->file, O_WRONLY | O_CREAT | O_APPEND, 0644);
            if (fd < 0) {
                perror(cur->file);
                exit(1);
            }
            if (dup2(fd, STDOUT_FILENO) < 0)
                perror_and_exit("dup2");
            close(fd);
        }
        cur = cur->right;
    }
}