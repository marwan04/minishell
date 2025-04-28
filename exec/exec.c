/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eaqrabaw <eaqrabaw@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/17 18:58:37 by malrifai          #+#    #+#             */
/*   Updated: 2025/04/28 22:20:18 by eaqrabaw         ###   ########.fr       */
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

int handle_cmd_node(t_ast *node, int prev_fd, t_minishell *data)
{
	pid_t	pid;
	int		status;
	int    in_pipe ;
	
	if (prev_fd != -1)
		in_pipe = 1;
	else 
		in_pipe = 0;
	if (is_builtin(node->args[0]) && !in_pipe)
	{
		execute_builtin_cmds(node, &data->last_exit_status, &data->env);
		return (0);
	}
	signal(SIGINT, SIG_IGN);
	signal(SIGQUIT, SIG_IGN);
	pid = fork();
	if (pid == 0)
	{
		signal(SIGINT, SIG_DFL);
		signal(SIGQUIT, SIG_DFL);
		if (prev_fd != -1)
		{
			dup2(prev_fd, STDIN_FILENO);
			close(prev_fd);
		}
		if (is_builtin(node->args[0]))
			execute_builtin_cmds(node, &data->last_exit_status, &data->env);
		else
			ft_execute_command(node, data);
		exit(data->last_exit_status);
	}
	else
	{
		if (prev_fd != -1)
			close(prev_fd);
		waitpid(pid, &status, 0);
		init_signals();
		data->last_exit_status = WEXITSTATUS(status);
	}
	return (data->last_exit_status);
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

int	exec_ast(t_ast *node, int prev_fd, t_minishell *data)
{
	if (!node)
		return (1);
	if (node->type == NODE_AND || node->type == NODE_OR)
		return (exec_and_or(node, prev_fd, data));
	else if (node->type == NODE_GROUP)
		return (exec_ast(node->left, prev_fd, data));
	return (exec_leaf(node, prev_fd, data));
}
