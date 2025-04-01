/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_pipes.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: malrifai <malrifai@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/24 08:05:54 by eaqrabaw          #+#    #+#             */
/*   Updated: 2025/04/01 20:04:54 by malrifai         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

// void	ft_exec_cmd1(t_cmd *cmd, int pipefd[2], t_env *env, int *last_exit_status)
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

// void	ft_exec_cmd2(t_cmd *cmd, int pipefd[2], t_env *env, int *last_exit_status)
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

void	exec_pipes(t_cmd *cmds, int *last_exit_status, t_env **env)
{
	int		pipefd[2];
	int		prev_fd;
	pid_t	pid;
	int		status;
	t_cmd	*current = cmds;
	int		is_last;
	int		child_count;

	prev_fd = -1;
	child_count = 0;
	while (current)
	{
		if (current->next == NULL)
			is_last = 1;
		else
			is_last = 0;
		if (!is_last && pipe(pipefd) == -1)
		{
			perror("pipe");
			*last_exit_status = 1;
			return;
		}
		pid = fork();
		if (pid == -1)
		{
			perror("fork");
			*last_exit_status = 1;
			return;
		}
		if (pid == 0)
		{
			if (prev_fd != -1)
			{
				if (dup2(prev_fd, STDIN_FILENO) == -1)
					exit(1);
				close(prev_fd);
			}
			if (!is_last)
			{
				if (dup2(pipefd[1], STDOUT_FILENO) == -1)
					exit(1);
				close(pipefd[0]);
				close(pipefd[1]);
			}
			if (is_builtin(current->args[0]))
				execute_builtin_cmds(current, last_exit_status, env);
			else
				ft_execute_command(current, last_exit_status, env);
			exit(*last_exit_status);
		}
		if (prev_fd != -1)
			close(prev_fd);
		if (!is_last)
		{
			close(pipefd[1]);
			prev_fd = pipefd[0];
		}
		current = current->next;
		child_count++;
	}
	while (child_count-- > 0)
		wait(&status);
	ft_set_exit_status(last_exit_status, status);
}
