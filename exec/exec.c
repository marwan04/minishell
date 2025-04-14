/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eaqrabaw <eaqrabaw@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/17 18:58:37 by malrifai          #+#    #+#             */
/*   Updated: 2025/04/14 08:41:59 by eaqrabaw         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	execute_builtin_cmds(t_minishell *data)
{
	if (!data->cmds || !data->cmds->args)
	{
		data->last_exit_status = 100;
		return;
	}
	data->last_exit_status = 0;
	if (ft_strcmp(data->cmds->args[0], "pwd") == 0)
		handle_pwd();
	else if (ft_strcmp(data->cmds->args[0], "env") == 0)
		handle_env(data->env);
	else if (ft_strcmp(data->cmds->args[0], "echo") == 0)
		handle_echo(data->cmds->args);
	else if (ft_strcmp(data->cmds->args[0], "export") == 0)
		handle_export(data->cmds->args, &data->env);
	else if (ft_strcmp(data->cmds->args[0], "cd") == 0)
		handle_cd(data->cmds->args, &data->env);
	else if (ft_strcmp(data->cmds->args[0], "unset") == 0)
		handle_unset(data->cmds->args, &data->env);
}

int	ft_execute_command(t_minishell *data)
{
	char	*full_path;
	char	**envp;

	if (initialize_execution_params(&full_path, &envp, data->cmds->args, &data->env) == -1)
		return (-1);
	execve(full_path, data->cmds->args, envp);
	free(full_path);
	ft_free_double_list(envp);
	ft_perror("Execve Failed", 5);
	data->last_exit_status = 127;
	return (-1);
}

void	ft_execute(t_minishell *data)
{
	pid_t	pid_id;
	int		status;

	if (data->cmds->next && data->cmds->next->pipe != 0)
	{
		exec_pipes(data);
		return;
	}
	if (is_builtin(data->cmds->args[0]))
	{
		execute_builtin_cmds(data);
		return;
	}
	pid_id = fork();
	if (pid_id == -1)
	{
		perror("fork");
		data->last_exit_status = 1;
		return;
	}
	if (pid_id == 0)
	{
		if (ft_execute_command(data) == -1)
			exit(data->last_exit_status);
	}
	waitpid(pid_id, &status, 0);
	ft_set_exit_status(&data->last_exit_status, status);
}
