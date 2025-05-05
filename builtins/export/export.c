/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: malrifai <malrifai@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/17 18:10:00 by malrifai          #+#    #+#             */
/*   Updated: 2025/05/05 18:51:12 by malrifai         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_valid_identifier(char *key)
{
	int	i;

	if (!key || !ft_isalpha(key[0]) || key[0] == '=')
		return (0);
	i = 0;
	while (key[i])
	{
		if (!ft_isalnum(key[i]) && key[i] != '_')
			return (0);
		i++;
	}
	return (1);
}

int	handle_export_with_value(char *arg, t_env **env, char *equal)
{
	char	*key;
	char	*value;

	key = ft_substr(arg, 0, equal - arg);
	value = ft_strdup(equal + 1);
	if (!key || !value)
	{
		if (key)
			free(key);
		if (value)
			free(value);
		printf("bash: export: memory allocation failed\n");
		return (1);
	}
	if (!is_valid_identifier(key))
	{
		printf("bash: export: `%s': not a valid identifier\n", key);
		free(key);
		free(value);
		return (1);
	}
	add_or_update_env(env, key, value);
	free(key);
	free(value);
	return (0);
}

int	handle_export_without_value(char *arg)
{
	if (!is_valid_identifier(arg))
	{
		printf("bash: export: `%s': not a valid identifier\n", arg);
		return (1);
	}
	return (0);
}

int	set_env(char **args, t_env **env, char *equal, int i)
{
	if (!equal)
		return (handle_export_without_value(args[i]));
	else
		return (handle_export_with_value(args[i], env, equal));
}

void	handle_export(char **args, t_env **env)
{
	char	*equal;
	int		i;

	if (!args[1])
	{
		print_env_sorted(*env);
		return ;
	}
	i = 1;
	while (args[i])
	{
		equal = ft_strchr(args[i], '=');
		if (set_env(args, env, equal, i))
		{
			i++;
			continue ;
		}
		i++;
	}
}
