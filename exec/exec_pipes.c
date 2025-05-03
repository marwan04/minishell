/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_pipes.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eaqrabaw <eaqrabaw@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/24 08:05:54 by eaqrabaw          #+#    #+#             */
/*   Updated: 2025/05/01 01:32:25 by eaqrabaw         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// Fixed function to safely close heredoc file descriptors
void close_all_heredoc_fds(t_ast *node)
{
    if (!node)
        return;

    // If this is a heredoc node, close its pipe if the fds are valid
    if (node->type == NODE_HEREDOC)
    {
        if (node->heredoc_pipe[0] >= 0)
        {
            close(node->heredoc_pipe[0]);
            node->heredoc_pipe[0] = -1;
        }
        
        if (node->heredoc_pipe[1] >= 0)
        {
            close(node->heredoc_pipe[1]);
            node->heredoc_pipe[1] = -1;  // Mark as closed to prevent double-close
        }
    }

    // Recursively close heredoc fds in the left and right subtrees
    close_all_heredoc_fds(node->left);
    close_all_heredoc_fds(node->right);
}

// Fixed function for child process pipe handling
void child_process_pipe(int prev_fd, int pipefd[2], t_ast *node, t_minishell *data)
{
    // Close unused pipe ends in child
    close(pipefd[0]);

    // Handle input redirection
    if (prev_fd != -1)
    {
        if (dup2(prev_fd, STDIN_FILENO) == -1)
        {
            perror("dup2");
            exit(1);
        }
        close(prev_fd);
    }
    else if (node->type == NODE_HEREDOC && node->heredoc_pipe[0] >= 0)
    {
        if (dup2(node->heredoc_pipe[0], STDIN_FILENO) == -1)
        {
            perror("dup2");
            exit(1);
        }
        close(node->heredoc_pipe[0]);
        node->heredoc_pipe[0] = -1;
    }

    // Handle output redirection
    if (dup2(pipefd[1], STDOUT_FILENO) == -1)
    {
        perror("dup2");
        exit(1);
    }
    close(pipefd[1]);

    // Execute the command
    if (exec_ast(node->left, -1, data) == -1)
    {
        perror("exec");
        exit(1);
    }
}

// Fixed function for pipe and fork
pid_t pipe_and_fork(int pipefd[2], int prev_fd, t_ast *node, t_minishell *data)
{
    pid_t pid;

    // Create the pipe for the current segment of the pipeline
    if (pipe(pipefd) == -1)
    {
        perror("pipe");
        close_all_heredoc_fds(node);
        return (-1);
    }

    // Fork the process
    pid = fork();
    if (pid == 0)
    {
        // Child process
        signal(SIGINT, SIG_DFL);
        signal(SIGQUIT, SIG_DFL);
        child_process_pipe(prev_fd, pipefd, node, data);
        // The child process will not return from child_process_pipe
    }
    else if (pid < 0)
    {
        perror("fork");
        close(pipefd[0]);
        close(pipefd[1]);
        close_all_heredoc_fds(node);
        return (-1);
    }

    // Parent process
    close(pipefd[1]); // Close write end of pipe in parent
    return pid;
}

// Fixed function for handling pipe nodes
int handle_pipe_node(t_ast *node, int prev_fd, t_minishell *data)
{
    int pipefd[2];
    pid_t pid;
    int status;

    // Create pipe
    if (pipe(pipefd) == -1)
    {
        perror("pipe");
        return (-1);
    }

    // Fork child process
    pid = fork();
    if (pid == -1)
    {
        perror("fork");
        close(pipefd[0]);
        close(pipefd[1]);
        return (-1);
    }
    else if (pid == 0)
    {
        // Child process
        close(pipefd[0]);  // Close read end in child
        
        // Execute left command and free its AST
        t_ast *left = node->left;
        node->left = NULL;
        child_process_pipe(prev_fd, pipefd, left, data);
        free_ast(left);
        
        // Free this node before exiting
        free_ast(node);
        exit(1);
    }
    else
    {
        // Parent process
        close(pipefd[1]);  // Close write end in parent
        if (prev_fd != -1)
            close(prev_fd);

        // Wait for the child process
        if (waitpid(pid, &status, 0) == -1)
        {
            perror("waitpid");
            close(pipefd[0]);
            return (-1);
        }

        // Update last exit status
        if (WIFEXITED(status))
            data->last_exit_status = WEXITSTATUS(status);
        else if (WIFSIGNALED(status))
            data->last_exit_status = 128 + WTERMSIG(status);

        // Handle next command in pipeline
        if (node->right)
        {
            // Execute right command and free its AST
            t_ast *right = node->right;
            node->right = NULL;
            int next_status = handle_pipe_node(right, pipefd[0], data);
            close(pipefd[0]);
            free_ast(right);
            return next_status;
        }

        // Last command in pipeline
        close(pipefd[0]);
        
        // Free this node
        free_ast(node);
        return 0;
    }
    return 0;
}