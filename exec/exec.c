/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eaqrabaw <eaqrabaw@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/17 18:58:37 by malrifai          #+#    #+#             */
/*   Updated: 2025/05/02 06:00:04 by eaqrabaw         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	execute_builtin_cmds(t_ast *node, int *last_exit_status, t_env **env)
{
	if (!node || !node->args)
	{
		*last_exit_status = 100;
		return ;
	}
	*last_exit_status = 0;
	if (ft_strcmp(node->args[0], "pwd") == 0)
		handle_pwd();
	else if (ft_strcmp(node->args[0], "env") == 0)
		handle_env(*env);
	else if (ft_strcmp(node->args[0], "echo") == 0)
		handle_echo(node->args);
	else if (ft_strcmp(node->args[0], "export") == 0)
		handle_export(node->args, env);
	else if (ft_strcmp(node->args[0], "cd") == 0)
		handle_cd(node->args, env);
	else if (ft_strcmp(node->args[0], "unset") == 0)
		handle_unset(node->args, env);
}

static int	exec_and_or(t_ast *node, int prev_fd, t_minishell *data)
{
	exec_ast(node->left, prev_fd, data);
	if ((node->type == NODE_AND && data->last_exit_status == 0)
		|| (node->type == NODE_OR && data->last_exit_status != 0))
		exec_ast(node->right, prev_fd, data);
	return (data->last_exit_status);
}

static int	exec_leaf(t_ast *node, int prev_fd, t_minishell *data)
{
	if (node->type == NODE_PIPE)
		return (handle_pipe_node(node, prev_fd, data));
	else if (node->type == NODE_CMD)
		return (handle_cmd_node(node, prev_fd, data));
	else if (node->type == NODE_REDIR_IN || node->type == NODE_REDIR_OUT
		|| node->type == NODE_APPEND)
		return (handle_redirection_node(node, prev_fd, data));
	else if (node->type == NODE_HEREDOC)
		return (handle_heredoc_node(node, prev_fd, data));
	return (0);
}
int exec_ast(t_ast *node, int prev_fd, t_minishell *data)
{
    if (!node)
        return (1);

    // ─── Pipeline ──────────────────────────────────────────────────────────
    if (node->type == NODE_PIPE)
    {
        int pipefd[2];
        if (pipe(pipefd) == -1)
        {
            perror("pipe");
            return (-1);
        }

        pid_t pid = fork();
        if (pid == -1)
        {
            perror("fork");
            close(pipefd[0]);
            close(pipefd[1]);
            return (-1);
        }
        else if (pid == 0)
        {
            // Child: read from prev_fd, write to pipefd[1]
            close(pipefd[0]);
            if (prev_fd != -1)
            {
                dup2(prev_fd, STDIN_FILENO);
                close(prev_fd);
            }
            dup2(pipefd[1], STDOUT_FILENO);
            close(pipefd[1]);
        
            // Execute left command and free its AST
            t_ast *left = node->left;
            node->left = NULL;
            exec_ast(left, -1, data);
            free_ast(left);
            
            // Free this node before exiting
            free_ast(node);
            exit(1);
        }
        // Parent: close write end, optionally close prev_fd
        close(pipefd[1]);
        if (prev_fd != -1)
            close(prev_fd);

        int status;
        waitpid(pid, &status, 0);
        if (WIFEXITED(status))
            data->last_exit_status = WEXITSTATUS(status);
        else if (WIFSIGNALED(status))
            data->last_exit_status = 128 + WTERMSIG(status);

        // Recurse into the right side of the pipeline
        if (node->right)
        {
            int next_status = exec_ast(node->right, pipefd[0], data);
            close(pipefd[0]);
            
            // Free the right node after execution
            t_ast *right = node->right;
            node->right = NULL;
            free_ast(right);
            
            return next_status;
        }

        close(pipefd[0]);
        
        // Free this node
        free_ast(node);
        return (data->last_exit_status);
    }

    // ─── && / || ────────────────────────────────────────────────────────────
    if (node->type == NODE_AND || node->type == NODE_OR)
        return exec_and_or(node, prev_fd, data);

    // ─── Grouping ───────────────────────────────────────────────────────────
    if (node->type == NODE_GROUP)
        return exec_ast(node->left, prev_fd, data);

    // ─── Leaf (simple command) ─────────────────────────────────────────────
    return exec_leaf(node, prev_fd, data);
}
