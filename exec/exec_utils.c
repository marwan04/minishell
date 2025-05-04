/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: malrifai <malrifai@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/04 08:18:26 by eaqrabaw          #+#    #+#             */
/*   Updated: 2025/05/05 01:11:42 by malrifai         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"  

void	set_exit_status_from_wait(int status, t_minishell *data)
{
	int	sig;

	if (WIFSIGNALED(status))
	{
		sig = WTERMSIG(status);
		if (sig == SIGINT)
			data->last_exit_status = 130;
		else if (sig == SIGQUIT)
			data->last_exit_status = 131;
		else
			data->last_exit_status = 128 + sig;
	}
	else
		data->last_exit_status = WEXITSTATUS(status);
}

static size_t	count_env(t_env *env)
{
	size_t	count;
	t_env	*tmp;

	count = 0;
	tmp = env;
	while (tmp)
	{
		count++;
		tmp = tmp->next;
	}
	return (count);
}

static char	*make_entry(t_env *env)
{
	size_t	klen;
	size_t	vlen;
	char	*entry;

	klen = ft_strlen(env->key);
	vlen = 0;
	if (env->value)
		vlen = ft_strlen(env->value);
	entry = malloc(klen + 1 + vlen + 1);
	if (!entry)
		return (NULL);
	ft_strlcpy(entry, env->key, klen + 1);
	entry[klen] = '=';
	if (env->value)
		ft_strlcpy(entry + klen + 1, env->value, vlen + 1);
	else
		entry[klen + 1] = '\0';
	return (entry);
}

static void	free_arr(char **arr, size_t idx)
{
	while (idx > 0)
	{
		idx--;
		free(arr[idx]);
	}
	free(arr);
}

char	**envp_to_array(t_env *env)
{
	size_t	count;
	char	**arr;
	size_t	idx;
	char	*entry;

	count = count_env(env);
	arr = malloc((count + 1) * sizeof(char *));
	if (!arr)
		return (NULL);
	idx = 0;
	while (env)
	{
		entry = make_entry(env);
		if (!entry)
		{
			free_arr(arr, idx);
			return (NULL);
		}
		arr[idx] = entry;
		idx++;
		env = env->next;
	}
	arr[idx] = NULL;
	free_env(env);
	return (arr);
}
