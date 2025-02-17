/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: malrifai <malrifai@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/10 17:59:26 by malrifai          #+#    #+#             */
/*   Updated: 2025/02/17 19:11:32 by malrifai         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

char	*ft_strjoin_free(char *s1, char *s2)
{
	char	*joined;

	joined = ft_strjoin(s1, s2);
	free(s1);
	return (joined);
}

char	**ft_realloc_env(char **env, char *key, char *value)
{
	int		i;
	char	**new_env;
	char	*new_var;

	i = 0;
	while (env[i])
		i++;
	new_env = malloc((i + 2) * sizeof(char *));
	if (!new_env)
		return (NULL);
	i = 0;
	while (env[i])
	{
		new_env[i] = ft_strdup(env[i]);
		free(env[i]);
		i++;
	}
	new_var = ft_strjoin_free(ft_strjoin(key, "="), value);
	new_env[i++] = new_var;
	new_env[i] = NULL;
	free(env);
	return (new_env);
}

char	**ft_dup_env(char **av)
{
	int		i;
	char	**new_env;

	i = 0;
	while (av[i])
		i++;
	new_env = malloc((i + 1) * sizeof(char *));
	if (!new_env)
		return (NULL);
	i = 0;
	while (av[i])
	{
		new_env[i] = ft_strdup(av[i]);
		i++;
	}
	new_env[i] = NULL;
	return (new_env);
}
