/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: malrifai <malrifai@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/18 15:49:41 by malrifai          #+#    #+#             */
/*   Updated: 2025/02/19 17:58:00 by malrifai         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

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

void	free_env(t_env *env)
{
	t_env	*tmp;

	while (env)
	{
		tmp = env->next;
		free(env->key);
		free(env->value);
		free(env);
		env = tmp;
	}
}

t_env	*init_env_list(char **envp)
{
	t_env	*head;
	t_env	*new_node;
	char	*equal;
	int		i;

	i = 0;
	head = NULL;
	while (envp[i])
	{
		new_node = malloc(sizeof(t_env));
		equal = ft_strchr(envp[i], '=');
		if (equal)
		{
			new_node->key = ft_substr(envp[i], 0, equal - envp[i]);
			new_node->value = ft_strdup(equal + 1);
		}
		else
		{
			new_node->key = ft_strdup(envp[i]);
			new_node->value = NULL;
		}
		new_node->next = head;
		head = new_node;
		i++;
	}
	return (head);
}

void	add_or_update_env(t_env **env, char *key, char *value)
{
	t_env	*tmp;
	t_env	*new_node;

	tmp = *env;
	while (tmp)
	{
		if (ft_strcmp(tmp->key, key) == 0)
		{
			free(tmp->value);
			tmp->value = ft_strdup(value);
			return ;
		}
		tmp = tmp->next;
	}
	new_node = malloc(sizeof(t_env));
	new_node->key = ft_strdup(key);
	new_node->value = ft_strdup(value);
	new_node->next = *env;
	*env = new_node;
}

char	**build_env(t_env *env)
{
	int		size;
	t_env	*tmp;
	char	**env_array;
	char	*entry;

	size = 0;
	tmp = env;
	while (tmp)
	{
		size++;
		tmp = tmp->next;
	}
	env_array = malloc(sizeof(char *) * (size + 1));
	tmp = env;
	size = 0;
	while (tmp)
	{
		entry = ft_strjoin(tmp->key, "=");
		env_array[size] = ft_strjoin(entry, tmp->value);
		free(entry);
		size++;
		tmp = tmp->next;
	}
	env_array[size] = NULL;
	return (env_array);
}
