/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eaqrabaw <eaqrabaw@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/17 18:58:37 by malrifai          #+#    #+#             */
/*   Updated: 2025/02/23 10:38:42 by eaqrabaw         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "../includes/minishell.h"

void	execute_cmds(t_cmd *cmds, int *last_exit_status, t_env **env)
{
	
	if (!cmds || !cmds->args)
		return ;
	if (ft_strcmp(cmds->args[0], "pwd") == 0)
	{
		handle_pwd();
		*last_exit_status = 0;
		return ;
	}
	if (ft_strcmp(cmds->args[0], "env") == 0)
    {
        handle_env(*env);
        *last_exit_status = 0;
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
