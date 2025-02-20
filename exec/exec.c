/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alrfa3i <alrfa3i@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/17 18:58:37 by malrifai          #+#    #+#             */
/*   Updated: 2025/02/18 00:41:37 by alrfa3i          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	execute_cmds(t_cmd *cmds, int *last_exit_status, char ***env)
{
	if (!cmds || !cmds->args)
	{
		return ;
	}
	if (ft_strcmp(cmds->args[0], "echo") == 0)
	{
		handle_echo(cmds->args);
		*last_exit_status = 0;
		return ;
	}
	if (ft_strcmp(cmds->args[0], "export") == 0)
	{
		handle_export(cmds->args, env);
		*last_exit_status = 0;
		return ;
	}
}
