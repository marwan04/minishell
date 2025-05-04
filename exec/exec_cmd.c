/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_cmd.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eaqrabaw <eaqrabaw@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/04 07:26:17 by eaqrabaw          #+#    #+#             */
/*   Updated: 2025/05/04 07:29:33 by eaqrabaw         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int exec_cmd_node(t_ast *node, int prev_fd, t_minishell *data)
{
    int   status;
    pid_t pid;

    if (!node || !node->args)
        return 0;
    if (data->last_exit_status == 130) 
        return (data->last_exit_status = 130);
    if (node->args[0] && is_builtin(node->args[0]) && 
        prev_fd == STDIN_FILENO && 
        node->type == NODE_CMD && !node->right) {
        execute_builtin_cmds(node, &status, &data->env);
        return (data->last_exit_status = status);
    }
    pid = fork();
    if (pid < 0) {
        perror("fork");
        if (prev_fd > STDERR_FILENO)
            close(prev_fd);
        return (data->last_exit_status = 1);
    }
    if (pid == 0) {
        if (prev_fd > STDIN_FILENO) {
            if (dup2(prev_fd, STDIN_FILENO) < 0)
                perror_and_exit("dup2");
            close(prev_fd);
        }
        apply_redirections(node);
        if (node->args && node->args[0]) {
            if (is_builtin(node->args[0])) {
                int exit_code;
                execute_builtin_cmds(node, &exit_code, &data->env);
                exit(exit_code);
            } else {
                char *path = ft_get_path(node->args[0], &data->env);
                if (!path) {
                    fprintf(stderr, "%s: command not found\n", node->args[0]);
                    exit(127);
                }
                char **envp = envp_to_array(data->env);
                if (!envp) {
                    perror("malloc");
                    exit(126);
                }
                execve(path, node->args, envp);
                perror("execve");
                free(path);
                for (int i = 0; envp[i]; i++)
                    free(envp[i]);
                free(envp);
                exit(126);
            }
        }
        exit(0);
    }
    if (prev_fd > STDERR_FILENO)
        close(prev_fd);
    waitpid(pid, &status, 0);
    if (WIFSIGNALED(status)) {
        int sig = WTERMSIG(status);
        if (sig == SIGINT)
            data->last_exit_status = 130;
        else if (sig == SIGQUIT)
            data->last_exit_status = 131;
        else
            data->last_exit_status = 128 + sig;
    } else {
        data->last_exit_status = WEXITSTATUS(status);
    }
    return data->last_exit_status;
}
