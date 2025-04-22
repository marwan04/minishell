/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: malrifai <malrifai@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/14 14:55:50 by malrifai          #+#    #+#             */
/*   Updated: 2025/03/14 15:44:35 by malrifai         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	unset_env_variable(t_env **env, char *key)
{
	t_env	*current;
	t_env	*prev;

	current = *env;
	prev = NULL;
	while (current)
	{
		if (ft_strcmp(current->key, key) == 0)
		{
			if (prev)
				prev->next = current->next;
			else
				*env = current->next;
			free(current->key);
			free(current->value);
			free(current);
			return ;
		}
		prev = current;
		current = current->next;
	}
}

void	handle_unset(char **args, t_env **env)
{
	int	i;

	i = 1;
	if (!args[i])
		return ;
	while (args[i])
	{
		if (!is_valid_identifier(args[i]))
			printf("bash: unset: `%s': not a valid identifier\n", args[i]);
		else
			unset_env_variable(env, args[i]);
		i++;
	}
}
