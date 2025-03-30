/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_pipes.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: malrifai <malrifai@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/24 08:05:54 by eaqrabaw          #+#    #+#             */
/*   Updated: 2025/03/30 15:07:41 by malrifai         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	ft_exec_cmd1(t_cmd *cmd, int pipefd[2], t_env *env, int *last_exit_status)
{
	dup2(pipefd[1], STDOUT_FILENO);
	close(pipefd[0]);
	close(pipefd[1]);
	if (is_builtin(cmd->args[0]))
		execute_builtin_cmds(cmd, last_exit_status, &env);
	else
		ft_execute_command(cmd, last_exit_status, &env);
	exit(*last_exit_status);
}

void	ft_exec_cmd2(t_cmd *cmd, int pipefd[2], t_env *env, int *last_exit_status)
{
	dup2(pipefd[0], STDIN_FILENO);
	close(pipefd[0]);
	close(pipefd[1]);
	if (is_builtin(cmd->args[0]))
		execute_builtin_cmds(cmd, last_exit_status, &env);
	else
		ft_execute_command(cmd, last_exit_status, &env);
	exit(*last_exit_status);
}

void	exec_pipes(t_cmd *cmds, int *last_exit_status, t_env **env)
{
	int		pipefd[2];
	pid_t	pid1;
	pid_t	pid2;
	int		status;

	if (!cmds || !cmds->next)
		return ;
	if (pipe(pipefd) == -1)
	{
		perror("pipe");
		*last_exit_status = 1;
		return;
	}
	pid1 = fork();
	if (pid1 == -1)
	{
		perror("fork");
		*last_exit_status = 1;
		return;
	}
	if (pid1 == 0)
		ft_exec_cmd1(cmds, pipefd, *env, last_exit_status);
	pid2 = fork();
	if (pid2 == -1)
	{
		perror("fork");
		*last_exit_status = 1;
		return;
	}
	if (pid2 == 0)
		ft_exec_cmd2(cmds->next, pipefd, *env, last_exit_status);
	close(pipefd[0]);
	close(pipefd[1]);
	waitpid(pid1, &status, 0);
	waitpid(pid2, &status, 0);
	ft_set_exit_status(last_exit_status, status);
}
