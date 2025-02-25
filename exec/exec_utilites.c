/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_utilites.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eaqrabaw <eaqrabaw@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/24 10:07:13 by eaqrabaw          #+#    #+#             */
/*   Updated: 2025/02/25 08:35:05 by eaqrabaw         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int initialize_execution_params(char **full_path, char ***envp, char **args, t_env **env)
{
    *envp = build_env(*(env));
    if (!envp)
        return (-1);
    if (access(args[0], X_OK) == 0)
        *full_path = ft_strdup(args[0]);
    else
        *full_path = ft_get_path(args[0], env);
    if (!*full_path)
    {
        ft_free_double_list((*envp));
        return (-1);
    }
    return (0);
}
