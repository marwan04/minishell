/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eaqrabaw <eaqrabaw@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/17 18:58:37 by malrifai          #+#    #+#             */
/*   Updated: 2025/02/24 11:54:16 by eaqrabaw         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "../includes/minishell.h"

void	execute_builtin_cmds(t_cmd *cmds, int *last_exit_status, t_env **env)
{
	if (!cmds || !cmds->args)
	{
		*last_exit_status = 100;
		return ;
	}
	*last_exit_status = 0;
	if (ft_strcmp(cmds->args[0], "pwd") == 0)
		handle_pwd();
	else if (ft_strcmp(cmds->args[0], "env") == 0)
        handle_env(*env);
	else if (ft_strcmp(cmds->args[0], "pwd") == 0)
		handle_pwd();
	else if (ft_strcmp(cmds->args[0], "env") == 0)
        handle_env(*env);
	else if (ft_strcmp(cmds->args[0], "echo") == 0)
		handle_echo(cmds->args);
	else if (ft_strcmp(cmds->args[0], "export") == 0)
		handle_export(cmds->args, env);
	return ;
}

int ft_execute_command(t_cmd *cmds, int *last_exit_status, t_env **env)
{
    char *full_path;
    char **full_cmd;
    char **envp;

    envp = build_env(*(env));
    if (!envp)
        return (-1);
    if (initialize_execution_params(&full_cmd, &full_path, cmds->args[0], env) == -1)
    {
        ft_free_double_list(envp);
        return (-1);
    }
    execve(full_path, full_cmd, envp);
    ft_perror("Execve Failed", 5);
    free(full_path);
    ft_free_double_list(envp);
    ft_free_double_list(full_cmd);
	*last_exit_status = -1;
    return (-1);
}

void    ft_execute(t_cmd *cmds, int *last_exit_status, t_env **env)
{
    pid_t  pidId;
    int    status;
    
    // if (cmds->pipe != 0)
    // {
    //     ft_execute_pipes(&cmds, &last_exit_status, &env); // need to be created
    //     return ;
    // }
    pidId = fork();
    if (pidId == -1)
    {
        *last_exit_status = -1;
        return ;
    }
    if (pidId == 0)
    {
        // if (cmds->has_redirection)
        //     ft_execute_with_redirections();   // need to be created 
        // else
            ft_execute_command(cmds, last_exit_status, env);
    }
    waitpid(pidId, &status, 0);
    ft_set_exit_status(last_exit_status, status);
    return ;
}
