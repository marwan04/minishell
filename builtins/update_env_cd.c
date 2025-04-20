/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   update_env_cd.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: malrifai <malrifai@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/12 18:48:56 by malrifai          #+#    #+#             */
/*   Updated: 2025/04/12 18:51:03 by malrifai         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*get_new_cwd_or_fail(char *oldpwd, int should_free)
{
	char	*newpwd;

	newpwd = getcwd(NULL, 0);
	if (!newpwd)
	{
		perror("cd: getcwd");
		if (should_free)
			free(oldpwd);
	}
	return (newpwd);
}

char	*get_current_pwd_or_empty(t_env *env, int *should_free)
{
	char	*oldpwd;

	*should_free = 0;
	oldpwd = get_env_value(env, "PWD");
	if (!oldpwd)
	{
		oldpwd = ft_strdup("");
		if (!oldpwd)
		{
			perror("cd: malloc");
			return (NULL);
		}
		*should_free = 1;
	}
	return (oldpwd);
}

int	update_pwd_env(t_env **env)
{
	char	*oldpwd;
	char	*newpwd;
	int		should_free_oldpwd;

	oldpwd = get_current_pwd_or_empty(*env, &should_free_oldpwd);
	if (!oldpwd)
		return (1);
	newpwd = get_new_cwd_or_fail(oldpwd, should_free_oldpwd);
	if (!newpwd)
		return (1);
	add_or_update_env(env, "OLDPWD", oldpwd);
	add_or_update_env(env, "PWD", newpwd);
	if (should_free_oldpwd)
		free(oldpwd);
	free(newpwd);
	return (0);
}
