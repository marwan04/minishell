/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eaqrabaw <eaqrabaw@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/17 18:58:37 by malrifai          #+#    #+#             */
/*   Updated: 2025/04/20 10:13:16 by eaqrabaw         ###   ########.fr       */
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

int	ft_execute_command(t_ast *node, int *last_exit_status, t_env **env)
{
	char	*full_path;
	char	**envp;

	if (initialize_execution_params(&full_path, &envp, node->args, env) == -1)
		return (-1);
	execve(full_path, node->args, envp);
	free(full_path);
	ft_free_double_list(envp);
	ft_perror("Execve Failed", 5);
	*last_exit_status = 127;
	return (-1);
}

int	handle_cmd_node(t_ast *node, int prev_fd, t_minishell *data)
{
	pid_t	pid;
	int		status;

	if (is_builtin(node->args[0]))
	{
		execute_builtin_cmds(node, &data->last_exit_status, &data->env);
		return (0);
	}
	pid = fork();
	if (pid == 0)
	{
		if (prev_fd != -1)
		{
			dup2(prev_fd, STDIN_FILENO);
			close(prev_fd);
		}
		ft_execute_command(node, &data->last_exit_status, &data->env);
		exit(data->last_exit_status);
	}
	if (prev_fd != -1)
		close(prev_fd);
	waitpid(pid, &status, 0);
	data->last_exit_status = WEXITSTATUS(status);
	return (data->last_exit_status);
}

int	exec_ast(t_ast *node, int prev_fd, t_minishell *data)
{
	if (!node)
		return (1);
	if (node->type == NODE_PIPE)
		return (handle_pipe_node(node, prev_fd, data));
	else if (node->type == NODE_CMD || node->type == NODE_HEREDOC)
		return (handle_cmd_node(node, prev_fd, data));
	else if (node->type == NODE_REDIR_IN
		|| node->type == NODE_REDIR_OUT
		|| node->type == NODE_APPEND)
		return (handle_redirection_node(node, prev_fd, data));
	return (0);
}

// int	exec_ast(t_ast *node, int prev_fd, t_minishell *data)
// {
// 	int pipefd[2];
// 	pid_t pid;
// 	int status;

// 	if (!node)
// 		return 1;

// 	// Handle pipe nodes
// 	if (node->type == NODE_PIPE)
// 	{
// 		if (pipe(pipefd) == -1)
// 		{
// 			perror("pipe");
// 			return 1;
// 		}

// 		// Fork left side
// 		pid = fork();
// 		if (pid == 0)
// 		{
// 			if (prev_fd != -1)
// 			{
// 				dup2(prev_fd, STDIN_FILENO);
// 				close(prev_fd);
// 			}
// 			dup2(pipefd[1], STDOUT_FILENO);
// 			close(pipefd[0]);
// 			close(pipefd[1]);
// 			exec_ast(node->left, -1, data); // recursive call for left child
// 			exit(data->last_exit_status);
// 		}

// 		// Close write, prepare to use read side as prev_fd
// 		close(pipefd[1]);
// 		if (prev_fd != -1)
// 			close(prev_fd);

// 		// Fork right side
// 		exec_ast(node->right, pipefd[0], data); // use read side as stdin
// 		waitpid(pid, &status, 0);
// 		data->last_exit_status = WEXITSTATUS(status);
// 		return data->last_exit_status;
// 	}
// 	// Handle command execution
// 	if (node->type == NODE_CMD)
// 	{
// 		pid = fork();
// 		if (pid == 0)
// 		{
// 			if (prev_fd != -1)
// 			{
// 				dup2(prev_fd, STDIN_FILENO);
// 				close(prev_fd);
// 			}
// 			if (is_builtin(node->args[0]))
// 				execute_builtin_cmds(node, &data->last_exit_status, &data->env);
// 			else
// 				ft_execute_command(node, &data->last_exit_status, &data->env);
// 			exit(data->last_exit_status);
// 		}
// 		if (prev_fd != -1)
// 			close(prev_fd);
// 		waitpid(pid, &status, 0);
// 		data->last_exit_status = WEXITSTATUS(status);
// 	}
// 	return data->last_exit_status;
// }
