/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eaqrabaw <eaqrabaw@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/18 15:49:41 by malrifai          #+#    #+#             */
/*   Updated: 2025/04/20 09:52:42 by eaqrabaw         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	free_env_node(t_env *node)
{
	if (node)
	{
		free(node->key);
		free(node->value);
		free(node);
	}
}

char	**ft_free_env_array(char **env_array, int last)
{
	while (last--)
		free(env_array[last]);
	free(env_array);
	return (NULL);
}

void	free_env(t_env *env)
{
	t_env	*tmp;

	while (env)
	{
		tmp = env->next;
		free_env_node(env);
		env = tmp;
	}
}

static t_env	*s_create_env_node(char *envp_entry)
{
	t_env	*new_node;
	char	*equal;

	new_node = malloc(sizeof(t_env));
	if (!new_node)
		return (NULL);
	equal = ft_strchr(envp_entry, '=');
	if (equal)
	{
		new_node->key = ft_substr(envp_entry, 0, equal - envp_entry);
		new_node->value = ft_strdup(equal + 1);
	}
	else
	{
		new_node->key = ft_strdup(envp_entry);
		new_node->value = NULL;
	}
	if (!new_node->key || (equal && !new_node->value))
	{
		free_env_node(new_node);
		return (NULL);
	}
	new_node->next = NULL;
	return (new_node);
}

t_env	*init_env_list(char **envp)
{
	t_env	*head;
	t_env	*new_node;
	int		i;

	head = NULL;
	i = 0;
	while (envp[i])
	{
		new_node = s_create_env_node(envp[i]);
		if (!new_node)
		{
			free_env(head);
			return (NULL);
		}
		new_node->next = head;
		head = new_node;
		i++;
	}
	return (head);
}
