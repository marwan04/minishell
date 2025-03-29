/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: malrifai <malrifai@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/05 22:12:06 by malrifai          #+#    #+#             */
/*   Updated: 2025/03/17 17:47:17 by malrifai         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	update_pwd_env(t_env **env)
{
	char	*oldpwd;
	char	*newpwd;

	oldpwd = get_env_value(*env, "PWD");
	newpwd = getcwd(NULL, 0);
	if (!newpwd || !oldpwd)
		exit(0);
	add_or_update_env(env, "OLDPWD", oldpwd);
	add_or_update_env(env, "PWD", newpwd);
	free(newpwd);
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
	update_pwd_env(env);
}
