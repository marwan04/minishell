/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_pipes.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eaqrabaw <eaqrabaw@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/24 08:05:54 by eaqrabaw          #+#    #+#             */
/*   Updated: 2025/03/24 08:11:00 by eaqrabaw         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void  	ft_exec_cmd1()
{
	
}

void  	ft_exec_cmd2()
{
	
}

void	exec_pipes(t_cmd *cmds, int *last_exit_status, t_env **env)
{
	int 	pipefd[2];
	int		status;
	
	if (pipe(pipefd) == -1)
	{
		*last_exit_status = -1;
		return ;
	}
	
}
