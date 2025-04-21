/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   update_env.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: malrifai <malrifai@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/12 18:44:34 by malrifai          #+#    #+#             */
/*   Updated: 2025/04/12 18:45:38 by malrifai         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	create_and_add_env_node(t_env **env, char *key, char *value)
{
	t_env	*new_node;

	new_node = malloc(sizeof(t_env));
	if (!new_node)
		return ;
	new_node->key = ft_strdup(key);
	new_node->value = ft_strdup(value);
	if (!new_node->key || !new_node->value)
	{
		free_env_node(new_node);
		return ;
	}
	new_node->next = *env;
	*env = new_node;
}

void	update_existing_env(t_env *node, char *value)
{
	char	*new_val;

	new_val = ft_strdup(value);
	if (!new_val)
	{
		perror("malloc");
		return ;
	}
	free(node->value);
	node->value = new_val;
}

void	add_or_update_env(t_env **env, char *key, char *value)
{
	t_env	*tmp;

	tmp = *env;
	while (tmp)
	{
		if (ft_strcmp(tmp->key, key) == 0)
		{
			update_existing_env(tmp, value);
			return ;
		}
		tmp = tmp->next;
	}
	create_and_add_env_node(env, key, value);
}
