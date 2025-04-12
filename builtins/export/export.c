/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: malrifai <malrifai@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/17 18:10:00 by malrifai          #+#    #+#             */
/*   Updated: 2025/04/12 18:13:40 by malrifai         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

// int	count_equal(char *str)
// {
// 	int	count;
// 	int	i;

// 	count = 0;
// 	i = 0;
// 	while (str[i])
// 	{
// 		if (str[i] == '=')
// 			count++;
// 		i++;
// 	}
// 	return (count);
// }

// void	handle_export(char **args, t_env **env)
// {
// 	char	*equal;
// 	char	*key;
// 	char	*value;
// 	int		res;

// 	if (!args[1])
// 	{
// 		print_env_sorted(*env);
// 		return ;
// 	}
// 	equal = ft_strchr(args[1], '=');
// 	if (!equal)
// 	{
// 		if (!is_valid_identifier(args[1]))
// 		{
// 			printf("bash: export: `%s': not a valid identifier\n", args[1]);
// 			add_or_update_env(env, args[1], "");
// 			return ;
// 		}
// 	}
// 	else
// 	{
// 		res = has_space_after_equal(args[1]);
// 		printf("res: %d\n", res);
// 		key = ft_substr(args[1], 0, equal - args[1]);
// 		if (has_space_before_equal(args[1]) || res == 1)
// 		{
// 			printf("bash: export: `%s': not a valid identifier\n", args[1]);
// 			add_or_update_env(env, key, "");
// 			free(key);
// 			return ;
// 		}
// 		if (res == 2)
// 		{
// 			add_or_update_env(env, key, "");
// 			free(key);
// 			return ;
// 		}
// 		value = ft_strdup(equal + 1);
// 		add_or_update_env(env, key, value);
// 		free(key);
// 		free(value);
// 	}
// }

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
/*
int	set_env(char **args, t_env **env, char *equal, int i)
{
	char	*key;
	char	*value;

	if (!equal)
	{
		if (!is_valid_identifier(args[i]))
		{
			printf("bash: export: `%s': not a valid identifier\n", args[i]);
			return (1);
		}
	}
	else
	{
		key = ft_substr(args[i], 0, equal - args[i]);
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
	}
	return (0);
}
*/

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
