/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alrfa3i <alrfa3i@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/17 18:10:00 by malrifai          #+#    #+#             */
/*   Updated: 2025/02/18 00:48:38 by alrfa3i          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	print_env_vars(char **env)
{
	int	i;

	i = 0;
	while (env[i])
	{
		printf("declare -x %s\n", env[i]);
		i++;
	}
}

void	export_variable(char *arg, char ***env)
{
	char	*key;
	char	*value;
	int		i;

	i = 0;
	key = ft_substr(arg, 0, ft_strchr(arg, '=') - arg);
	value = ft_strdup(ft_strchr(arg, '=') + 1);
	while ((*env)[i])
	{
		if (ft_strncmp((*env)[i], key, ft_strlen(key)) == 0
			&& (*env)[i][ft_strlen(key)] == '=')
		{
			free((*env)[i]);
			(*env)[i] = ft_strjoin_free(ft_strjoin(key, "="), value);
			free(key);
			free(value);
			return ;
		}
		i++;
	}
	(*env) = ft_realloc_env(*env, key, value);
	free(key);
	free(value);
}

void	handle_export(char **args, char ***env)
{
	int		i;
	char	*var_value;

	if (!args[1])
	{
		print_env_vars(*env);
		return ;
	}
	i = 1;
	while (args[i])
	{
		if (ft_strchr(args[i], '='))
			export_variable(args[i], env);
		else
		{
			var_value = getenv(args[i]);
			if (var_value)
				printf("declare -x %s=\"%s\"\n", args[i], var_value);
		}
		i++;
	}
}
