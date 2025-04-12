/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_env.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: malrifai <malrifai@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/22 18:00:15 by malrifai          #+#    #+#             */
/*   Updated: 2025/04/12 18:21:56 by malrifai         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

// t_env	*copy_env(t_env *original)
// {
// 	t_env	*copy;
// 	t_env	*tail;
// 	t_env	*new_node;

// 	copy = NULL;
// 	tail = NULL;
// 	while (original)
// 	{
// 		new_node = malloc(sizeof(t_env));
// 		if (!new_node)
// 		{
// 			free_env(copy);
// 			return (NULL);
// 		}
// 		new_node->key = ft_strdup(original->key);
// 		if (original->value)
// 			new_node->value = ft_strdup(original->value);
// 		else
// 			new_node->value = NULL;
// 		new_node->next = NULL;
// 		if (!copy)
// 		{
// 			copy = new_node;
// 			tail = new_node;
// 		}
// 		else
// 		{
// 			tail->next = new_node;
// 			tail = new_node;
// 		}
// 		original = original->next;
// 	}
// 	return (copy);
// }

void	is_larger(int *swapped, t_env **ptr)
{
	char	*tmp_key;
	char	*tmp_value;

	if (ft_strcmp((*ptr)->key, (*ptr)->next->key) > 0)
	{
		tmp_key = (*ptr)->key;
		tmp_value = (*ptr)->value;
		(*ptr)->key = (*ptr)->next->key;
		(*ptr)->value = (*ptr)->next->value;
		(*ptr)->next->key = tmp_key;
		(*ptr)->next->value = tmp_value;
		*swapped = 1;
	}
}

void	sort_env_list(t_env **env)
{
	int		swapped;
	t_env	*ptr;
	t_env	*last;

	last = NULL;
	swapped = 1;
	if (!env || !*env)
	{
		return ;
	}
	while (swapped)
	{
		swapped = 0;
		ptr = *env;
		while (ptr->next != last)
		{
			is_larger(&swapped, &ptr);
			ptr = ptr->next;
		}
		last = ptr;
	}
}

void	print_env_sorted(t_env *env)
{
	t_env	*copy;
	t_env	*to_free;

	copy = copy_env(env);
	to_free = copy;
	if (!copy)
	{
		printf("bash: export: failed to allocate memory for sorting\n");
		return ;
	}
	sort_env_list(&copy);
	while (copy)
	{
		if (copy->value)
			printf("declare -x %s=\"%s\"\n", copy->key, copy->value);
		else
			printf("declare -x %s\n", copy->key);
		copy = copy->next;
	}
	free_env(to_free);
}
