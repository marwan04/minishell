/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_pipe.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eaqrabaw <eaqrabaw@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/04 12:00:00 by student           #+#    #+#             */
/*   Updated: 2025/05/04 23:01:54 by eaqrabaw         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "minishell.h"

int
exec_pipeline(t_ast **stages, int n_stages, int prev_fd, t_minishell *data)
{
    int   (*pipes)[2] = NULL;
    pid_t *pids        = NULL;
    int    status;
    int    last_status = 0;
    int    pipe_count  = n_stages - 1;

    if (n_stages <= 1)
        return exec_cmd_node(stages[0], prev_fd, data);
    pipes = malloc(pipe_count * sizeof *pipes);
    pids  = malloc(n_stages  * sizeof *pids);
    if (!pipes || !pids)
    {
        perror("malloc");
        free(pipes);
        free(pids);
        return 1;
    }
    for (int i = 0; i < pipe_count; ++i)
    {
        if (pipe(pipes[i]) < 0)
        {
            perror("pipe");
            for (int j = 0; j < i; ++j)
                close(pipes[j][0]), close(pipes[j][1]);
            free(pipes);
            free(pids);
            return 1;
        }
    }
    for (int i = 0; i < n_stages; ++i)
    {
        pid_t pid = fork();
        if (pid < 0)
        {
            perror("fork");
			free(pipes);
			free(pids);
			last_status = 1;
			return last_status;
        }

        if (pid == 0)
        {
            signal(SIGINT,  SIG_DFL);
            signal(SIGQUIT, SIG_DFL);
            apply_redirections(stages[i]);
            close_heredoc_pipes_in_stages(stages, n_stages);
            int used_in = 0, used_out = 0;
            for (t_ast *r = stages[i];
                 r && (r->type == NODE_HEREDOC
                     || r->type == NODE_REDIR_IN
                     || r->type == NODE_REDIR_OUT
                     || r->type == NODE_APPEND);
                 r = r->right)
            {
                if (r->type == NODE_HEREDOC || r->type == NODE_REDIR_IN)
                    used_in = 1;
                if (r->type == NODE_REDIR_OUT || r->type == NODE_APPEND)
                    used_out = 1;
            }
            for (int j = 0; j < pipe_count; ++j)
            {
                int rd = pipes[j][0], wr = pipes[j][1];
                if (j == i - 1)
                {
                    if (!used_in)
                        dup2(rd, STDIN_FILENO);
                    close(rd);
                    close(wr);
                }
                else if (j == i)
                {
                    close(rd);
                    if (!used_out)
                        dup2(wr, STDOUT_FILENO);
                    close(wr);
                }
                else
                {
                    close(rd);
                    close(wr);
                }
            }

            if (prev_fd > STDERR_FILENO)
                close(prev_fd);
            t_ast *cmd = stages[i];
            while (cmd && (cmd->type == NODE_HEREDOC
                        || cmd->type == NODE_REDIR_IN
                        || cmd->type == NODE_REDIR_OUT
                        || cmd->type == NODE_APPEND))
                cmd = cmd->left;
            if (cmd && cmd->args && cmd->args[0])
            {
                if (is_builtin(cmd->args[0]))
                {
                    int ret = 0;
                    execute_builtin_cmds(cmd, &ret, &data->env);
                    ft_free(data, ret, NULL);
                }
                else
                {
                    char *path = ft_get_path(cmd->args[0], &data->env);
                    if (!path)
                    {
                        ft_putstr_fd(cmd->args[0], 2);
                        ft_putstr_fd(": command not found\n", 2);
                        ft_free(data, 127, NULL);
                    }
                    char **envp_arr = envp_to_array(data->env);
                    execve(path, cmd->args, envp_arr);
                    perror("execve");
                    free(path);
                    ft_free_double_list(envp_arr);
                    ft_free(data, 126, NULL);
                }
            }
            ft_free(data, 1, NULL);
        }
        pids[i] = pid;
    }
    if (prev_fd > STDERR_FILENO)
        close(prev_fd);
    for (int j = 0; j < pipe_count; ++j)
        close(pipes[j][0]), close(pipes[j][1]);
    close_heredoc_pipes_in_stages(stages, n_stages);
    for (int i = 0; i < n_stages; ++i)
    {
        if (waitpid(pids[i], &status, 0) < 0)
            perror("waitpid");
        else
            last_status = WEXITSTATUS(status);
    }
    free(pipes);
    free(pids);
    return last_status;
}
