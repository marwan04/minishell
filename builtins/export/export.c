/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: malrifai <malrifai@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/17 18:10:00 by malrifai          #+#    #+#             */
/*   Updated: 2025/02/23 20:39:51 by malrifai         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"
int count_equal(char *str)
{
	int count = 0;
	int i = 0;
	while (str[i])
	{
		if (str[i] == '=')
			count++;
		i++;
	}
	return (count);
}
void handle_export(char **args, t_env **env)
{
	char	*equal;
	char	*key;
	char	*value;
	int		res;

	if (!args[1])
	{
		print_env_sorted(*env);
		return ;
	}
	equal = ft_strchr(args[1], '=');
	if (!equal)
	{
		if (!is_valid_identifier(args[1]))
		{
			printf("bash: export: `%s': not a valid identifier\n", args[1]);
			add_or_update_env(env, args[1], "");
			return ;
		}
	}
	else
	{
		res = has_space_after_equal(args[1]);
		printf("res: %d\n", res);
		key = ft_substr(args[1], 0, equal - args[1]);
		if (has_space_before_equal(args[1]) || res == 1)
		{
			printf("bash: export: `%s': not a valid identifier\n", args[1]);
			add_or_update_env(env, key, "");
			free(key);
			return ;
		}
		if (res == 2)
		{
			add_or_update_env(env, key, "");
			free(key);
			return ;
		}
		value = ft_strdup(equal + 1);
		add_or_update_env(env, key, value);
		free(key);
		free(value);
	}
}
