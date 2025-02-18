/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: malrifai <malrifai@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/17 18:10:00 by malrifai          #+#    #+#             */
/*   Updated: 2025/02/18 16:00:15 by malrifai         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	handle_export(char **args, t_env **env)
{
	int		i;
	char	*equal;
	char	*key;
	char	*value;
	t_env	*tmp;

	i = 0;
	if (!args[i])
	{
		tmp = *env;
		while (tmp)
		{
			printf("declare -x %s=\"%s\"\n", tmp->key, tmp->value);
			tmp = tmp->next;
		}
		return ;
	}
	while (args[i])
	{
		equal = ft_strchr(args[i], '=');
		if (!equal)
			add_or_update_env(env, args[i], "");
		else
		{
			key = ft_substr(args[i], 0, equal - args[i]);
			value = ft_strdup(equal + 1);
			add_or_update_env(env, key, value);
			free(key);
			free(value);
		}
		i++;
	}
}
