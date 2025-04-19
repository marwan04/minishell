/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_pipes.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: malrifai <malrifai@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/24 08:05:54 by eaqrabaw          #+#    #+#             */
/*   Updated: 2025/04/19 13:36:57 by malrifai         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

// void	ft_exec_cmd1(t_cmd *cmd,
// int pipefd[2], t_env *env, int *last_exit_status)
// {
// 	if (dup2(pipefd[1], STDOUT_FILENO) == -1)
// 	{
// 		perror("dup2");
// 		exit(1);
// 	}
// 	close(pipefd[0]);
// 	close(pipefd[1]);
// 	if (is_builtin(cmd->args[0]))
// 		execute_builtin_cmds(cmd, last_exit_status, &env);
// 	else
// 		ft_execute_command(cmd, last_exit_status, &env);
// 	exit(*last_exit_status);
// }

// void	ft_exec_cmd2(t_cmd *cmd,
//int pipefd[2], t_env *env, int *last_exit_status)
// {
// 	if (dup2(pipefd[0], STDIN_FILENO) == -1)
// 	{
// 		perror("dup2");
// 		exit(1);
// 	}
// 	close(pipefd[0]);
// 	close(pipefd[1]);
// 	if (is_builtin(cmd->args[0]))
// 		execute_builtin_cmds(cmd, last_exit_status, &env);
// 	else
// 		ft_execute_command(cmd, last_exit_status, &env);
// 	exit(*last_exit_status);
// }

// int	handle_pipe_node(t_ast *node, int prev_fd, t_minishell *data)
// {
// 	int		pipefd[2];
// 	int		status;
// 	pid_t	pid;

// 	if (pipe(pipefd) == -1)
// 	{
// 		perror("pipe");
// 		return (1);
// 	}
// 	pid = fork();
// 	if (pid == 0)
// 	{
// 		if (prev_fd != -1)
// 		{
// 			dup2(prev_fd, STDIN_FILENO);
// 			close(prev_fd);
// 		}
// 		dup2(pipefd[1], STDOUT_FILENO);
// 		close(pipefd[0]);
// 		close(pipefd[1]);
// 		exec_ast(node->left, -1, data);
// 		ft_free(data, 1, "");
// 	}
// 	close(pipefd[1]);
// 	if (prev_fd != -1)
// 		close(prev_fd);
// 	exec_ast(node->right, pipefd[0], data);
// 	waitpid(pid, &status, 0);
// 	data->last_exit_status = WEXITSTATUS(status);
// 	return (data->last_exit_status);
// }

void	child_process_pipe(int prev_fd, int pipefd[2],
			t_ast *node, t_minishell *data)
{
	if (prev_fd != -1)
	{
		dup2(prev_fd, STDIN_FILENO);
		close(prev_fd);
	}
	dup2(pipefd[1], STDOUT_FILENO);
	close(pipefd[0]);
	close(pipefd[1]);
	exec_ast(node->left, -1, data);
	ft_free(data, 1, "");
}

pid_t	pipe_and_fork(int pipefd[2], int prev_fd,
			t_ast *node, t_minishell *data)
{
	pid_t	pid;

	if (pipe(pipefd) == -1)
	{
		perror("pipe");
		return (-1);
	}
	pid = fork();
	if (pid == 0)
		child_process_pipe(prev_fd, pipefd, node, data);
	return (pid);
}

int	handle_pipe_node(t_ast *node, int prev_fd, t_minishell *data)
{
	int		pipefd[2];
	int		status;
	pid_t	pid;

	pid = pipe_and_fork(pipefd, prev_fd, node, data);
	if (pid == -1)
		return (1);
	close(pipefd[1]);
	if (prev_fd != -1)
		close(prev_fd);
	exec_ast(node->right, pipefd[0], data);
	waitpid(pid, &status, 0);
	data->last_exit_status = WEXITSTATUS(status);
	return (data->last_exit_status);
}
