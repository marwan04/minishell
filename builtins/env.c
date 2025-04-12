/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: malrifai <malrifai@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/22 15:54:59 by malrifai          #+#    #+#             */
/*   Updated: 2025/04/12 18:44:21 by malrifai         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	handle_env(t_env *env)
{
	while (env)
	{
		if (env->value)
			printf("%s=%s\n", env->key, env->value);
		env = env->next;
	}
}

char	*get_env_value(t_env *env, char *key)
{
	while (env)
	{
		if (ft_strcmp(env->key, key) == 0)
			return (env->value);
		env = env->next;
	}
	return (NULL);
}

static int	ft_env_size(t_env *env)
{
	t_env	*tmp;
	int		size;

	size = 0;
	tmp = env;
	while (tmp)
	{
		size++;
		tmp = tmp->next;
	}
	return (size);
}

char	**build_env(t_env *env)
{
	int		size;
	t_env	*tmp;
	char	**env_array;
	int		i;

	size = ft_env_size(env);
	env_array = malloc(sizeof(char *) * (size + 1));
	if (!env_array)
		return (NULL);
	tmp = env;
	i = 0;
	while (tmp)
	{
		env_array[i] = ft_strjoin_var(3, tmp->key, "=", tmp->value);
		if (!env_array[i])
			return (ft_free_env_array(env_array, i));
		i++;
		tmp = tmp->next;
	}
	env_array[i] = NULL;
	return (env_array);
}

// void	add_or_update_env(t_env **env, char *key, char *value)
// {
// 	t_env	*tmp;
// 	t_env	*new_node;
// 	char	*new_val;

// 	tmp = *env;
// 	while (tmp)
// 	{
// 		if (ft_strcmp(tmp->key, key) == 0)
// 		{
// 			new_val = ft_strdup(value);
// 			if (!new_val)
// 			{
// 				perror("malloc");
// 				return ;
// 			}
// 			free(tmp->value);
// 			tmp->value = new_val;
// 			return ;
// 		}
// 		tmp = tmp->next;
// 	}
// 	new_node = malloc(sizeof(t_env));
// 	if (!new_node)
// 		return ;
// 	new_node->key = ft_strdup(key);
// 	new_node->value = ft_strdup(value);
// 	if (!new_node->key || !new_node->value)
// 	{
// 		free_env_node(new_node);
// 		return ;
// 	}
// 	new_node->next = *env;
// 	*env = new_node;
// }