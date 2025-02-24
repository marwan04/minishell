/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_utilites.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eaqrabaw <eaqrabaw@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/24 10:07:13 by eaqrabaw          #+#    #+#             */
/*   Updated: 2025/02/24 10:07:52 by eaqrabaw         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int initialize_execution_params(char ***full_cmd, char **full_path, char *args, t_env **env)
{
    *full_cmd = ft_split(args + 1, " ");
    if (!*full_cmd)
    {
        ft_perror("Invalid Command", 1);
        return -1;
    }

    if (access(args, X_OK) == 0)
        *full_path = ft_strdup(args);
    else
        *full_path = ft_get_path(args, env);
    
    if (!*full_path)
    {
        ft_free_double_list(*full_cmd);
        return -1;
    }
    return 0;
}