/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eaqrabaw <eaqrabaw@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/18 15:49:41 by malrifai          #+#    #+#             */
/*   Updated: 2025/04/06 19:04:45 by eaqrabaw         ###   ########.fr       */
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
	t_env	*head = NULL;
	t_env	*new_node;
	char	*equal;
	int		i;

	i = 0;
	while (envp[i])
	{
		new_node = malloc(sizeof(t_env));
		if (!new_node)
			return (free_env(head), NULL);
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
		if (!new_node->key || (equal && !new_node->value))
		{
			if (new_node->key) free(new_node->key);
			if (new_node->value) free(new_node->value);
			free(new_node);
			free_env(head);
			return (NULL);
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
			char *new_val = ft_strdup(value);
			if (!new_val)
			{
				perror("malloc");
				return ;
			}
			free(tmp->value);
			tmp->value = new_val;
			return ;
		}
		tmp = tmp->next;
	}
	new_node = malloc(sizeof(t_env));
	if (!new_node)
		return ;
	new_node->key = ft_strdup(key);
	new_node->value = ft_strdup(value);
	if (!new_node->key || !new_node->value)
	{
		free(new_node->key);
		free(new_node->value);
		free(new_node);
		return ;
	}
	new_node->next = *env;
	*env = new_node;
}

char	**build_env(t_env *env)
{
	int		size;
	t_env	*tmp;
	char	**env_array;

	size = 0;
	tmp = env;
	while (tmp)
	{
		size++;
		tmp = tmp->next;
	}
	env_array = malloc(sizeof(char *) * (size + 1));
	if (!env_array)
		return (NULL);
	tmp = env;
	size = 0;
	while (tmp)
	{
		env_array[size] = ft_strjoin_var(3, tmp->key, "=", tmp->value);
		if (!env_array[size])
		{
			while (size--)
				free(env_array[size]);
			free(env_array);
			return (NULL);
		}
		size++;
		tmp = tmp->next;
	}
	env_array[size] = NULL;
	return (env_array);
}
