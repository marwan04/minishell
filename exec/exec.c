/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eaqrabaw <eaqrabaw@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/17 18:58:37 by malrifai          #+#    #+#             */
/*   Updated: 2025/05/04 08:31:43 by eaqrabaw         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int exec_ast(t_ast *node, int prev_fd, t_minishell *data)
{
    int status;
    
    if (!node)
        return (0);
    status = 0;
    collect_heredocs(node, data);
    if (data->last_exit_status == 130)
    {
        data->pipes_count = 0;
        if (prev_fd > STDERR_FILENO || prev_fd != -1)
            close(prev_fd);
        return (130);
    }
    else if (node->type == NODE_PIPE)
    {
        int n_stages;
        t_ast **stages = collect_pipeline_stages(node, &n_stages);
        if (!stages)
            return (1);
        int status = exec_pipeline(stages, n_stages, prev_fd, data);
        free(stages);
        return status;
    }      
    else if(node->type == NODE_AND || node->type == NODE_OR)
        return exec_and_or(node, prev_fd, data);
    else if (node->type == NODE_GROUP)
        {
            pid_t pid = fork();
            if (pid < 0) {
                perror("fork");
                if (prev_fd > STDERR_FILENO)
                    close(prev_fd);
                return (data->last_exit_status = 1);
            }
            if (pid == 0) {
                if (prev_fd > STDERR_FILENO && prev_fd != -1) {
                    if (dup2(prev_fd, STDIN_FILENO) < 0)
                        perror_and_exit("dup2");
                    close(prev_fd);
                }
                exit(exec_ast(node->left, STDIN_FILENO, data));
            }
            if (prev_fd > STDERR_FILENO)
                close(prev_fd);
            waitpid(pid, &status, 0);
            if (WIFSIGNALED(status))
                data->last_exit_status = 128 + WTERMSIG(status);
            else
                data->last_exit_status = WEXITSTATUS(status);
            return data->last_exit_status;
        }
    else if (node->type == NODE_CMD)
        return exec_cmd_node(node, prev_fd, data);
    else if(node->type == NODE_HEREDOC || node->type == NODE_REDIR_IN || node->type == NODE_REDIR_OUT || node->type == NODE_APPEND)
        {
            pid_t pid = fork();
            if (pid < 0) {
                perror("fork");
                if (prev_fd > STDERR_FILENO)
                    close(prev_fd);
                return (data->last_exit_status = 1);
            }
            if (pid == 0) {
                if (prev_fd > STDERR_FILENO) {
                    if (dup2(prev_fd, STDIN_FILENO) < 0)
                        perror_and_exit("dup2");
                    close(prev_fd);
                }
                apply_redirections(node);
                if (node->left)
                    exit(exec_ast(node->left, STDIN_FILENO, data));
                else
                    exit(0);
            }
            if (prev_fd > STDERR_FILENO)
                close(prev_fd);    
            waitpid(pid, &status, 0);
            if (WIFSIGNALED(status))
                data->last_exit_status = 128 + WTERMSIG(status);
            else
                data->last_exit_status = WEXITSTATUS(status);
            return data->last_exit_status;
        }
    else
    {
        fprintf(stderr, "minishell: Unknown AST node type: %d\n", node->type);
        if (prev_fd > STDERR_FILENO)
            close(prev_fd);
        return (data->last_exit_status = 1);
    }
}
