/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   path.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eaqrabaw <eaqrabaw@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/23 08:39:20 by eaqrabaw          #+#    #+#             */
/*   Updated: 2025/02/23 10:20:54 by eaqrabaw         ###   ########.fr       */
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

static char	**ft_split_path(char ***env)
{
	char	*path;
	char	**splitted;

	path = ft_get_path_from_env(*env);
	if (!path)
		return (NULL);
	splitted = ft_split(path, ':');
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

char	*get_path(t_cmd *cmds, char ***env)
{
	char	**paths;
	char	*full_path;
	int		i;

	i = 0;
	paths = ft_split_path(env);
	if (!paths)
		return (NULL);
	while (paths[i])
	{
		full_path = ft_build_path(paths[i], cmds->args[0]); // this have to be changed ...
		if (access(full_path, X_OK) == 0)
		{
			ft_free_double_list(paths);
			return (full_path);
		}
		free(full_path);
		i++;
	}
	ft_free_double_list(paths);
	return (NULL);
}
