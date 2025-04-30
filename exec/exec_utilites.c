/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_utilites.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eaqrabaw <eaqrabaw@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/24 10:07:13 by eaqrabaw          #+#    #+#             */
/*   Updated: 2025/04/30 03:21:11 by eaqrabaw         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int ft_execute_command(t_ast *node, t_minishell *data)
{
    char *full_path = NULL;
    char **envp = NULL;

    if (initialize_execution_params(&full_path, &envp, node->args, &data->env) == -1)
    {
        data->last_exit_status = 127;
        return -1;
    }
    if (!full_path)
    {
        ft_free_double_list(envp);
        ft_putstr_fd(node->args[0], 2);
        ft_putendl_fd(": command not found", 2);
        ft_free(data, 127, "");
        exit(127);
    }
    execve(full_path, node->args, envp);

    // Only if execve fails
    free(full_path);
    ft_free_double_list(envp);
    ft_putstr_fd(node->args[0], 2);
    ft_putendl_fd(": command not found", 2);
    ft_free(data, 127, "");
    exit(127);
    return 1;
}

int		is_builtin(char *cmd)
{
	if (!cmd)
		return (0);
	else if (ft_strcmp(cmd, "cd") == 0)
		return (1);
	else if (ft_strcmp(cmd, "echo") == 0)
		return (1);
	else if (ft_strcmp(cmd, "pwd") == 0)
		return (1);
	else if (ft_strcmp(cmd, "export") == 0)
		return (1);
	else if (ft_strcmp(cmd, "unset") == 0)
		return (1);
	else if (ft_strcmp(cmd, "env") == 0)
		return (1);
	else if (ft_strcmp(cmd, "exit") == 0)
		return (1);
	else
		return (0);
}

int	initialize_execution_params(char **full_path, char ***envp,
	char **args, t_env **env)
{
	*envp = build_env(*env);
	if (!*envp)
		return (-1);

	if (access(args[0], X_OK) == 0)
		*full_path = ft_strdup(args[0]);
	else
		*full_path = ft_get_path(args[0], env);
	return (0);
}
