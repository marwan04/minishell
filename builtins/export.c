/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: malrifai <malrifai@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/17 18:10:00 by malrifai          #+#    #+#             */
/*   Updated: 2025/02/22 17:16:23 by malrifai         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int is_valid_identifier(char *key)
{
	int i = 0;

	if (!key || !ft_isalpha(key[0]) || key[0] == '=')
		return (0);
	while (key[i])
	{
		if (!ft_isalnum(key[i]) && key[i] != '_')
			return (0);
		i++;
	}
	return (1);
}

int has_space_before_equal(char *arg)
{
	int i = 0;

	while (arg[i] && arg[i] != '=')
	{
		if (arg[i] == ' ')
			return (1);
		i++;
	}
	return (0);
}

int has_space_after_equal(char *equal)
{
	int i;
	char quote;

	i = 0;
	while (equal[i] != '=')
		i++;
	if (equal[i + 1] == ' ')
	{
		i++;
		while (equal[i] == ' ')
			i++;
		if (equal[i] == '"' || equal[i] == '\'')
		{
			i++;
			quote = equal[i];
			while (equal[i] && equal[i] != quote)
			{
				if (equal[i] == ' ')
					return (1);
				i++;
			}
		}
		else
			return (2);
	}
	return (0);
}

#include "../includes/minishell.h"

void sort_env_list(t_env **env)
{
    int     swapped;
    t_env   *ptr;
    t_env   *last = NULL;

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
            if (ft_strcmp(ptr->key, ptr->next->key) > 0)
            {
                char *tmp_key = ptr->key;
                char *tmp_value = ptr->value;
                ptr->key = ptr->next->key;
                ptr->value = ptr->next->value;
                ptr->next->key = tmp_key;
                ptr->next->value = tmp_value;

                swapped = 1;
            }
            ptr = ptr->next;
        }
        last = ptr;
	}
}

void print_env_sorted(t_env *env)
{
    sort_env_list(&env);
    while (env)
    {
        if (env->value)
            printf("declare -x %s=\"%s\"\n", env->key, env->value);
        else
            printf("declare -x %s\n", env->key);
        env = env->next;
    }
}

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
			if (res == 2)
			{
				key = ft_strdup(" ");
			}
			else if (has_space_before_equal(args[i]) || res == 1)
			{
				printf("bash1: export: `%s': not a valid identifier\n", args[i]);
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
