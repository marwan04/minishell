/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_utilites.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: malrifai <malrifai@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/24 10:07:13 by eaqrabaw          #+#    #+#             */
/*   Updated: 2025/03/05 23:24:23 by malrifai         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	initialize_execution_params(char **full_path, char ***envp,
	char **args, t_env **env)
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
