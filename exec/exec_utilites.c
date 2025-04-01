/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_utilites.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: malrifai <malrifai@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/24 10:07:13 by eaqrabaw          #+#    #+#             */
/*   Updated: 2025/04/01 16:06:15 by malrifai         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	is_builtin(char *cmd)
{
	if (!cmd)
		return (0);
	return (
		ft_strcmp(cmd, "cd") == 0 ||
		ft_strcmp(cmd, "echo") == 0 ||
		ft_strcmp(cmd, "pwd") == 0 ||
		ft_strcmp(cmd, "export") == 0 ||
		ft_strcmp(cmd, "unset") == 0 ||
		ft_strcmp(cmd, "env") == 0 ||
		ft_strcmp(cmd, "exit") == 0
	);
}

int	initialize_execution_params(char **full_path, char ***envp,
	char **args, t_env **env)
{
	*envp = build_env(*env);
	if (!*envp)
		return (-1); // memory allocation failed for envp
	if (access(args[0], X_OK) == 0)
	{
		*full_path = ft_strdup(args[0]);
		if (!*full_path)
		{
			ft_free_double_list(*envp);
			return (-1); // malloc failed
		}
	}
	else
	{
		*full_path = ft_get_path(args[0], env);
		if (!*full_path)
		{
			ft_free_double_list(*envp);
			return (-1); // command not found or malloc fail
		}
	}
	return (0);
}
