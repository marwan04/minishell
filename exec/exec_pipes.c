/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_pipes.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eaqrabaw <eaqrabaw@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/24 08:05:54 by eaqrabaw          #+#    #+#             */
/*   Updated: 2025/04/14 08:59:31 by eaqrabaw         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static void init_exec_vars(t_exec_vars *vars, t_minishell *data)
{
    vars->prev_fd = -1;
    vars->child_count = 0;
    vars->current = data->cmds;
}

static int setup_pipe(t_minishell *data, int pipefd[2], int is_last)
{
    if (!is_last && pipe(pipefd) == -1)
    {
        perror("pipe");
        data->last_exit_status = 1;
        return (0);
    }
    return (1);
}

static pid_t create_process(t_minishell *data)
{
    pid_t pid;

    pid = fork();
    if (pid == -1)
    {
        perror("fork");
        data->last_exit_status = 1;
    }
    return (pid);
}

static void handle_child(t_minishell *data, t_cmd *cmd,
                         int prev_fd, int pipefd[2], int is_last)
{
    if (prev_fd != -1)
    {
        if (dup2(prev_fd, STDIN_FILENO) == -1)
        {
            perror("dup2 STDIN_FILENO");
            exit(1);
        }
        close(prev_fd);
    }
    if (!is_last)
    {
        if (dup2(pipefd[1], STDOUT_FILENO) == -1)
        {
            perror("dup2 STDOUT_FILENO");
            exit(1);
        }
        close(pipefd[0]);
        close(pipefd[1]);
    }
    if (is_builtin(cmd->args[0]))
        execute_builtin_cmds(data);
    else
        ft_execute_command(data);
    free_env(data->env);
    ft_free(data, 1, "");
    exit(data->last_exit_status);
}

static void close_fds(int *prev_fd, int pipefd[2], int is_last)
{
    if (*prev_fd != -1)
        close(*prev_fd);
    if (!is_last)
    {
        close(pipefd[1]);
        *prev_fd = pipefd[0];
    }
}

static void wait_for_children(int count, int *status, int *exit_status)
{
    while (count-- > 0)
        wait(status);
    ft_set_exit_status(exit_status, *status);
}

static void exec_loop(t_minishell *data, t_exec_vars *vars)
{
    while (vars->current)
    {
        vars->is_last = (vars->current->next == NULL);
        if (!setup_pipe(data, vars->pipefd, vars->is_last))
            return ;
        vars->pid = create_process(data);
        if (vars->pid == -1)
            return ;
        if (vars->pid == 0)
            handle_child(data, vars->current,
                         vars->prev_fd, vars->pipefd, vars->is_last);
        close_fds(&vars->prev_fd, vars->pipefd, vars->is_last);
        vars->current = vars->current->next;
        vars->child_count++;
    }
}

void exec_pipes(t_minishell *data)
{
    t_exec_vars vars;
    int status;

    init_exec_vars(&vars, data);
    exec_loop(data, &vars);
    wait_for_children(vars.child_count, &status, &data->last_exit_status);
}
