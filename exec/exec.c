/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: malrifai <malrifai@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/17 18:58:37 by malrifai          #+#    #+#             */
/*   Updated: 2025/04/01 16:23:35 by malrifai         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	execute_builtin_cmds(t_cmd *cmds, int *last_exit_status, t_env **env)
{
	if (!cmds || !cmds->args)
	{
		*last_exit_status = 100;
		return;
	}
	*last_exit_status = 0;
	if (ft_strcmp(cmds->args[0], "pwd") == 0)
		handle_pwd();
	else if (ft_strcmp(cmds->args[0], "env") == 0)
		handle_env(*env);
	else if (ft_strcmp(cmds->args[0], "echo") == 0)
		handle_echo(cmds->args);
	else if (ft_strcmp(cmds->args[0], "export") == 0)
		handle_export(cmds->args, env);
	else if (ft_strcmp(cmds->args[0], "cd") == 0)
		handle_cd(cmds->args, env);
	else if (ft_strcmp(cmds->args[0], "unset") == 0)
		handle_unset(cmds->args, env);
}

int	ft_execute_command(t_cmd *cmds, int *last_exit_status, t_env **env)
{
	char	*full_path;
	char	**envp;

	if (initialize_execution_params(&full_path, &envp, cmds->args, env) == -1)
		return (-1);

	execve(full_path, cmds->args, envp);
	free(full_path);
	ft_free_double_list(envp);
	ft_perror("Execve Failed", 5);
	*last_exit_status = 127;
	return (-1);
}

void	ft_execute(t_cmd *cmds, int *last_exit_status, t_env **env)
{
	pid_t	pid_id;
	int		status;

	if (cmds->next && cmds->next->pipe != 0)
	{
		exec_pipes(cmds, last_exit_status, env);
		return;
	}
	if (is_builtin(cmds->args[0]))
	{
		execute_builtin_cmds(cmds, last_exit_status, env);
		return;
	}
	pid_id = fork();
	if (pid_id == -1)
	{
		perror("fork");
		*last_exit_status = 1;
		return;
	}
	if (pid_id == 0)
	{
		if (ft_execute_command(cmds, last_exit_status, env) == -1)
			exit(*last_exit_status);
	}
	waitpid(pid_id, &status, 0);
	ft_set_exit_status(last_exit_status, status);
}
