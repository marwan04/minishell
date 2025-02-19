/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: malrifai <malrifai@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/17 18:10:00 by malrifai          #+#    #+#             */
/*   Updated: 2025/02/19 23:59:05 by malrifai         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int is_valid_identifier(char *key)
{
	int i = 0;

	if (!key || !ft_isalpha(key[0]) || key[0] == '=')
		return (0);
	while (key[i])
	{
		if (!ft_isalnum(key[i]) && key[i] != '_')
			return (0);
		i++;
	}
	return (1);
}

int has_space_before_equal(char *arg)
{
	int i = 0;

	while (arg[i] && arg[i] != '=')
	{
		if (arg[i] == ' ')
			return (1);
		i++;
	}
	return (0);
}

int has_space_after_equal(char *equal)
{
	return (equal && equal[1] == ' ');
}

void print_env(t_env *env)
{
	while (env)
	{
		if (env->value)
			printf("declare -x %s=\"%s\"\n", env->key, env->value);
		else
			printf("declare -x %s\n", env->key); // ✅ Handle empty value case
		env = env->next;
	}
}


void handle_export(char **args, t_env **env)
{
	int		i;
	char	*equal;
	char	*key;
	char	*value;

	i = 0;
	if (!args[i+1])
	{
		print_env(*env);
		return ;
	}
	while (args[i])
	{
		equal = ft_strchr(args[i], '=');
		if (!equal)
		{
			if (!is_valid_identifier(args[i]))
				printf("bash: export: `%s': not a valid identifier\n", args[i]);
		}
		else
		{
			if (has_space_before_equal(args[i]) || has_space_after_equal(equal))
			{
				printf("bash: export: `%s': not a valid identifier\n", args[i]);
				i++;
				continue;
			}
			key = ft_substr(args[i], 0, equal - args[i]);
			value = ft_strdup(equal + 1);
			if (!is_valid_identifier(key))
				printf("bash: export: `%s': not a valid identifier\n", key);
			else
				add_or_update_env(env, key, value);
			free(key);
			free(value);
		}
		i++;
	}
}
