/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   copy_env.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: malrifai <malrifai@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/12 18:21:37 by malrifai          #+#    #+#             */
/*   Updated: 2025/05/05 18:52:03 by malrifai         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	append_env_node(t_env **head, t_env **tail, t_env *new_node)
{
	if (!*head)
	{
		*head = new_node;
		*tail = new_node;
	}
	else
	{
		(*tail)->next = new_node;
		*tail = new_node;
	}
}

t_env	*create_env_node(t_env *src)
{
	t_env	*new_node;

	new_node = malloc(sizeof(t_env));
	if (!new_node)
		return (NULL);
	new_node->key = ft_strdup(src->key);
	if (src->value)
		new_node->value = ft_strdup(src->value);
	else
		new_node->value = NULL;
	new_node->next = NULL;
	return (new_node);
}

t_env	*copy_env(t_env *original)
{
	t_env	*copy;
	t_env	*tail;
	t_env	*new_node;

	copy = NULL;
	tail = NULL;
	while (original)
	{
		new_node = create_env_node(original);
		if (!new_node)
		{
			free_env(copy);
			return (NULL);
		}
		append_env_node(&copy, &tail, new_node);
		original = original->next;
	}
	return (copy);
}
