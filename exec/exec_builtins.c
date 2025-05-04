/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_builtins.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: malrifai <malrifai@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/04 06:57:14 by eaqrabaw          #+#    #+#             */
/*   Updated: 2025/05/05 00:59:33 by malrifai         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_builtin(char *cmd)
{
	if (!cmd)
		return (0);
	else if (ft_strcmp(cmd, "pwd") == 0)
		return (1);
	else if (ft_strcmp(cmd, "env") == 0)
		return (1);
	else if (ft_strcmp(cmd, "echo") == 0)
		return (1);
	else if (ft_strcmp(cmd, "export") == 0)
		return (1);
	else if (ft_strcmp(cmd, "unset") == 0)
		return (1);
	else if (ft_strcmp(cmd, "cd") == 0)
		return (1);
	else if (ft_strcmp(cmd, "exit") == 0)
		return (1);
	else
		return (0);
}

void	execute_builtin_cmds(t_ast *node, int *last_exit, t_env **env)
{
	if (!node || !node->args || !node->args[0])
	{
		*last_exit = 1;
		return ;
	}
	*last_exit = 0;
	if (strcmp(node->args[0], "pwd") == 0)
		handle_pwd();
	else if (strcmp(node->args[0], "env") == 0)
		handle_env(*env);
	else if (strcmp(node->args[0], "echo") == 0)
		handle_echo(node->args);
	else if (strcmp(node->args[0], "export") == 0)
		handle_export(node->args, env);
	else if (strcmp(node->args[0], "unset") == 0)
		handle_unset(node->args, env);
	else if (strcmp(node->args[0], "cd") == 0)
		handle_cd(node->args, env);
	else if (strcmp(node->args[0], "exit") == 0)
		*last_exit = handle_exit(node->args, last_exit);
}

int	exec_builtin(t_ast *node, t_minishell *data, int *status)
{
	execute_builtin_cmds(node, status, &data->env);
	return (*status);
}
