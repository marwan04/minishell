/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   path.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: malrifai <malrifai@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/23 08:39:20 by eaqrabaw          #+#    #+#             */
/*   Updated: 2025/05/05 00:34:11 by malrifai         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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
	if (!splitted)
		return (NULL);
	if (!splitted[0])
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
	return (full_path);
}

char	*try_resolve_path(char **paths, char *cmd)
{
	char	*full_path;
	int		i;

	i = 0;
	while (paths[i])
	{
		full_path = ft_build_path(paths[i], cmd);
		if (!full_path)
		{
			i++;
			continue ;
		}
		if (access(full_path, X_OK) == 0)
			return (full_path);
		free(full_path);
		i++;
	}
	return (NULL);
}

char	*ft_get_path(char *cmd, t_env **envp)
{
	char	**paths;
	char	*resolved;

	if (!cmd)
		return (NULL);
	if (ft_strchr(cmd, '/'))
	{
		if (access(cmd, X_OK) == 0)
			return (ft_strdup(cmd));
		else
			return (NULL);
	}
	paths = ft_split_path(envp);
	if (!paths)
		return (NULL);
	resolved = try_resolve_path(paths, cmd);
	ft_free_double_list(paths);
	return (resolved);
}
