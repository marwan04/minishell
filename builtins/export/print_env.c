/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_env.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: malrifai <malrifai@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/22 18:00:15 by malrifai          #+#    #+#             */
/*   Updated: 2025/02/22 18:12:03 by malrifai         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void is_larger(int *swapped, t_env **ptr)
{
    char    *tmp_key;
    char    *tmp_value;
    
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
            is_larger(&swapped, &ptr);
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