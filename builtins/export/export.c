/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: malrifai <malrifai@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/17 18:10:00 by malrifai          #+#    #+#             */
/*   Updated: 2025/02/22 19:58:52 by malrifai         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void handle_export(char **args, t_env **env)
{
	int		i;
	char	*equal;
	char	*key;
	char	*value;
	int		res;

	i = 0;
	if (!args[i+1])
	{
		print_env_sorted(*env);
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
			res = has_space_after_equal(args[i]);
			printf("res: %d\n", res);
			if (res == 2)
			{
				key = ft_strdup(" ");
			}
			else if (has_space_before_equal(args[i]) || res == 1)
			{
				printf("bash: export: `%s': not a valid identifier\n", args[i]);
				i++;
				continue;
			}
			else
				key = ft_substr(args[i], 0, equal - args[i]);
			value = ft_strdup(equal + 1);
			add_or_update_env(env, key, value);
			free(key);
			free(value);
		}
		i++;
	}
}
