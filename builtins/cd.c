/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alrfa3i <alrfa3i@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/05 22:12:06 by malrifai          #+#    #+#             */
/*   Updated: 2025/03/31 01:13:32 by alrfa3i          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int update_pwd_env(t_env **env)
{
	char	*oldpwd;
	char	*newpwd;
	int		should_free_oldpwd;

	should_free_oldpwd = 0;
	oldpwd = get_env_value(*env, "PWD");
	if (!oldpwd)
	{
		oldpwd = ft_strdup("");
		if (!oldpwd)
		{
			perror("cd: malloc");
			return (1);
		}
		should_free_oldpwd = 1;
	}
	newpwd = getcwd(NULL, 0);
	if (!newpwd)
	{
		perror("cd: getcwd");
		if (should_free_oldpwd)
			free(oldpwd);
		return (1);
	}
	add_or_update_env(env, "OLDPWD", oldpwd);
	add_or_update_env(env, "PWD", newpwd);
	if (should_free_oldpwd)
		free(oldpwd);
	free(newpwd);
	return (0);
}



int	no_args(t_env **env, char **target_dir)
{
	*target_dir = get_env_value(*env, "HOME");
	if (!(*target_dir))
	{
		printf("cd: HOME not set\n");
		return (1);
	}
	return (0);
}

void	handle_cd(char **args, t_env **env)
{
	char	*target_dir;

	if (!args[1])
	{
		if (no_args(env, &target_dir) == 1)
			return ;
	}
	else if (ft_strcmp(args[1], "-") == 0)
	{
		target_dir = get_env_value(*env, "OLDPWD");
		if (!target_dir)
		{
			printf("cd: OLDPWD not set\n");
			return ;
		}
		printf("%s\n", target_dir);
	}
	else
		target_dir = args[1];
		if (chdir(target_dir) == -1)
	{
		perror("cd");
		return ;
	}
	if (update_pwd_env(env) != 0)
		return ;
}
