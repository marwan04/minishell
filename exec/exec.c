/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eaqrabaw <eaqrabaw@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/17 18:58:37 by malrifai          #+#    #+#             */
/*   Updated: 2025/05/04 08:02:14 by eaqrabaw         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char **envp_to_array(t_env *env)
{
    size_t count = 0;
    for (t_env *e = env; e; e = e->next)
        count++;
    char **arr = malloc((count + 1) * sizeof(char*));
    if (!arr) return NULL;
    size_t idx = 0;
    for (t_env *e = env; e; e = e->next)
    {
        size_t klen = strlen(e->key);
        size_t vlen = e->value ? strlen(e->value) : 0;
        char *entry = malloc(klen + 1 + vlen + 1);
        if (!entry) {
            while (idx > 0) {
                free(arr[--idx]);
            }
            free(arr);
            return NULL;
        }
        strcpy(entry, e->key);
        entry[klen] = '=';
        if (e->value) {
            strcpy(entry + klen + 1, e->value);
            entry[klen + 1 + vlen] = '\0';
        } else {
            entry[klen + 1] = '\0';
        }
        arr[idx++] = entry;
    }
    arr[idx] = NULL;
    return arr;
}

int exec_ast(t_ast *node, int prev_fd, t_minishell *data)
{
    if (!node)
        return 0;
    
    int status = 0;
    collect_heredocs(node, data);
    // Check if heredoc collection was interrupted
    if (data->last_exit_status == 130) {
        data->pipes_count = 0;
        if (prev_fd > STDERR_FILENO || prev_fd != -1)
            close(prev_fd);
        return 130;
    }

    if (node->type == NODE_PIPE)
    {
        int n_stages;
        t_ast **stages = collect_pipeline_stages(node, &n_stages);
        if (!stages)
            return 1;  // malloc failure

        int status = exec_pipeline(stages, n_stages, prev_fd, data);
        free(stages);
        return status;
    }

    // Handle different node types
    switch (node->type)
    {
            
        case NODE_AND:
        case NODE_OR:
            return exec_and_or(node, prev_fd, data);
            
        case NODE_GROUP:
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
            
        case NODE_CMD:
            return exec_cmd_node(node, prev_fd, data);
            
        case NODE_HEREDOC:
        case NODE_REDIR_IN:
        case NODE_REDIR_OUT:
        case NODE_APPEND:
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
                    
                    // Execute the left subtree if it exists
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
            
        default:
            // Unknown node type
            fprintf(stderr, "minishell: Unknown AST node type: %d\n", node->type);
            if (prev_fd > STDERR_FILENO)
                close(prev_fd);
            return (data->last_exit_status = 1);
    }
}
