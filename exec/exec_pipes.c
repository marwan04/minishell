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
    // Redirect input from previous command in the pipeline (if exists)
    if (prev_fd != -1)
    {
        dup2(prev_fd, STDIN_FILENO);
        close(prev_fd); // Close the previous pipe after it's used
    }

    // If this is a heredoc node, redirect input from the heredoc pipe
    if (node->type == NODE_HEREDOC && node->heredoc_pipe[0] >= 0)
    {
        dup2(node->heredoc_pipe[0], STDIN_FILENO);
        close(node->heredoc_pipe[0]); // Close the heredoc pipe after using it
        node->heredoc_pipe[0] = -1;   // Mark as closed
    }

    // Redirect output to the current pipe's write end
    dup2(pipefd[1], STDOUT_FILENO);
    
    // Close pipe file descriptors after they've been duplicated
    close(pipefd[0]); 
    close(pipefd[1]);

    // Execute the current command in the pipeline
    exec_ast(node->left, -1, data);
    
    // Properly clean up before exit
    ft_free(data, 1, "");
    exit(data->last_exit_status);
}

// Fixed function for pipe and fork
pid_t pipe_and_fork(int pipefd[2], int prev_fd, t_ast *node, t_minishell *data)
{
    pid_t pid;

    // Create the pipe for the current segment of the pipeline
    if (pipe(pipefd) == -1)
    {
        perror("pipe");
        return (-1);
    }

    // Fork the process
    pid = fork();
    if (pid == 0)
    {
        child_process_pipe(prev_fd, pipefd, node, data); // Child process handles the pipe
        // The child process will not return from child_process_pipe
    }
    else if (pid < 0)
    {
        perror("fork");
        close(pipefd[0]);
        close(pipefd[1]);
    }

    return pid;
}

// Fixed function for handling pipe nodes
int handle_pipe_node(t_ast *node, int prev_fd, t_minishell *data)
{
    int pipefd[2];
    int status;
    pid_t pid;

    pid = pipe_and_fork(pipefd, prev_fd, node, data);
    if (pid == -1)
        return (1); // Handle error

    // Parent closes the write end of the pipe
    close(pipefd[1]);

    // Close the previous file descriptor if it exists
    if (prev_fd > -1)
    {
        close(prev_fd);
    }

    // If this is a heredoc node, close its FDs in the parent
    if (node->type == NODE_HEREDOC)
    {
        // Close only if the fd is valid (not already closed)
        if (node->heredoc_pipe[0] >= 0)
        {
            close(node->heredoc_pipe[0]);
            node->heredoc_pipe[0] = -1; // Mark as closed
        }
        
        if (node->heredoc_pipe[1] >= 0)
        {
            close(node->heredoc_pipe[1]);
            node->heredoc_pipe[1] = -1; // Mark as closed
        }
    }
    exec_ast(node->right, pipefd[0], data);
    if (pipefd[0] >= 0)
        close(pipefd[0]);
    waitpid(pid, &status, 0);
    if (WIFEXITED(status))
        data->last_exit_status = WEXITSTATUS(status);
    else if (WIFSIGNALED(status))
        data->last_exit_status = 128 + WTERMSIG(status);
    else
        data->last_exit_status = 1;
    return data->last_exit_status;
}