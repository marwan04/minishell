/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   path.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eaqrabaw <eaqrabaw@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/23 08:39:20 by eaqrabaw          #+#    #+#             */
/*   Updated: 2025/03/24 09:09:50 by eaqrabaw         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static char	*ft_get_path_from_env(char **env)
{
	int	i;

	i = 0;
	while (env[i])
	{
		if (ft_strncmp(env[i], "PATH=", 5) == 0)
			return (env[i] + 5);
		i++;
	}
	return (NULL);
}

static char	**ft_split_path(t_env **envp)
{
	char	**env;
	char	*path;
	char	**splitted;

	env = build_env(*(envp));
	if (!env)
		return (NULL);
	path = ft_get_path_from_env(env);
	if (!path)
	{
		ft_free_double_list(env);
		return (NULL);
	}
	splitted = ft_split(path, ':');
	ft_free_double_list(env);
	if (!splitted || !splitted[0])
	{
		ft_free_double_list(splitted);
		return (NULL);
	}
	return (splitted);
}

static char	*ft_build_path(char *dir, char *cmd)
{
	char	*temp;
	char	*full_path;

	temp = ft_strjoin(dir, "/");
	if (!temp)
		return (NULL);
	full_path = ft_strjoin(temp, cmd);
	free(temp);
	if (!full_path)
		return (NULL);
	return (full_path);
}

char	*ft_get_path(char *cmd, t_env **envp)
{
	char	**paths;
	char	*full_path;
	int		i;

	if (!cmd)
		return (NULL);
	paths = ft_split_path(envp);
	if (!paths)
		return (NULL);
	i = -1;
	while (paths[++i])
	{
		full_path = ft_build_path(paths[i], cmd);
		if (!full_path)
			continue;
		if (access(full_path, X_OK) == 0)
		{
			ft_free_double_list(paths);
			return (full_path);
		}
		free(full_path);
	}
	ft_free_double_list(paths);
	return (NULL);
}
