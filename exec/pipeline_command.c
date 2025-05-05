/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipeline_command.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: malrifai <malrifai@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/05 14:02:39 by malrifai          #+#    #+#             */
/*   Updated: 2025/05/05 14:07:31 by malrifai         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	handle_external_command(t_ast *cmd, t_minishell *data)
{
	char	*path;
	char	**envp_arr;

	path = ft_get_path(cmd->args[0], &data->env);
	if (!path)
	{
		ft_putstr_fd(cmd->args[0], 2);
		ft_putstr_fd(": command not found\n", 2);
		ft_free(data, 127, NULL);
	}
	envp_arr = envp_to_array(data->env);
	execve(path, cmd->args, envp_arr);
	perror("execve");
	free(path);
	ft_free_double_list(envp_arr);
	ft_free(data, 126, NULL);
}

void	exec_handle_command(t_ast *cmd, t_minishell *data)
{
	int	ret;

	ret = 0;
	if (is_builtin(cmd->args[0]))
	{
		execute_builtin_cmds(cmd, &ret, &data->env);
		ft_free(data, ret, NULL);
	}
	else
		handle_external_command(cmd, data);
}
