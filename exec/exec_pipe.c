/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_pipe.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eaqrabaw <eaqrabaw@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/04 07:16:24 by eaqrabaw          #+#    #+#             */
/*   Updated: 2025/05/04 10:39:28 by eaqrabaw         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void destroy_pipeline_resources(int (*pipes)[2], int pipe_count, pid_t *pids)
{
    if (pipes)
    {
        for (int i = 0; i < pipe_count; i++)
        {
            close(pipes[i][0]);
            close(pipes[i][1]);
        }
        free(pipes);
    }
    if (pids)
        free(pids);
}

static t_ast *find_cmd_node(t_ast *node)
{
    while (node && (node->type == NODE_HEREDOC || node->type == NODE_REDIR_IN ||
                    node->type == NODE_REDIR_OUT || node->type == NODE_APPEND))
    {
        node = node->left;
    }
    return node;
}

static void handle_child_process(t_ast **stages, int i, int n_stages, int (*pipes)[2], int prev_fd, t_minishell *data)
{
    signal(SIGINT, SIG_DFL);
    signal(SIGQUIT, SIG_DFL);

    if (apply_redirections(stages[i]) < 0)
        exit(1);
    int used_in = 0, used_out = 0;
    for (t_ast *r = stages[i]; r && (r->type == NODE_HEREDOC || r->type == NODE_REDIR_IN || r->type == NODE_REDIR_OUT || r->type == NODE_APPEND); r = r->right)
    {
        if (r->type == NODE_HEREDOC || r->type == NODE_REDIR_IN) used_in = 1;
        if (r->type == NODE_REDIR_OUT || r->type == NODE_APPEND) used_out = 1;
    }
    for (int j = 0; j < n_stages - 1; j++)
    {
        if (j == i - 1 && !used_in)
            dup2(pipes[j][0], STDIN_FILENO);
        if (j == i && !used_out)
            dup2(pipes[j][1], STDOUT_FILENO);
        close(pipes[j][0]);
        close(pipes[j][1]);
    }
    if (prev_fd > STDERR_FILENO)
        close(prev_fd);
    t_ast *cmd = find_cmd_node(stages[i]);
    if (!cmd || !cmd->args || !cmd->args[0])
        exit(0);
    if (is_builtin(cmd->args[0]))
    {
        int ret = 0;
        execute_builtin_cmds(cmd, &ret, &data->env);
        exit(ret);
    }
    char *path = ft_get_path(cmd->args[0], &data->env);
    if (!path)
    {
        fprintf(stderr, "%s: command not found\n", cmd->args[0]);
        exit(127);
    }
    execve(path, cmd->args, envp_to_array(data->env));
    perror("execve");
    exit(126);
}

int exec_pipeline(t_ast **stages, int n_stages, int prev_fd, t_minishell *data)
{
    int    (*pipes)[2] = NULL;
    pid_t  *pids = NULL;
    int     status, last_status = 0;
    int     pipe_count = n_stages - 1;

    if (n_stages <= 1)
        return exec_cmd_node(stages[0], prev_fd, data);

    pipes = malloc(pipe_count * sizeof *pipes);
    pids  = malloc(n_stages * sizeof *pids);
    if (!pipes || !pids)
    {
        perror("malloc");
        destroy_pipeline_resources(pipes, 0, pids);
        return 1;
    }

    for (int i = 0; i < pipe_count; i++)
    {
        if (pipe(pipes[i]) < 0)
        {
            perror("pipe");
            destroy_pipeline_resources(pipes, i, pids);
            return 1;
        }
    }

    for (int i = 0; i < n_stages; i++)
    {
        pid_t pid = fork();
        if (pid < 0)
        {
            perror("fork");
            destroy_pipeline_resources(pipes, pipe_count, pids);
            return 1;
        }

        if (pid == 0)
            handle_child_process(stages, i, n_stages, pipes, prev_fd, data);

        pids[i] = pid;
    }

    if (prev_fd > STDERR_FILENO)
        close(prev_fd);

    destroy_pipeline_resources(pipes, pipe_count, NULL);

    for (int i = 0; i < n_stages; i++)
    {
        if (waitpid(pids[i], &status, 0) < 0)
        {
            perror("waitpid");
            last_status = 1;
        }
        else
        {
            last_status = WEXITSTATUS(status);
        }
    }

    close_heredoc_pipes_in_stages(stages, n_stages);
    free(pids);
    return last_status;
}
