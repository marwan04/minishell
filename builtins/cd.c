/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: malrifai <malrifai@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/05 22:12:06 by malrifai          #+#    #+#             */
/*   Updated: 2025/04/30 16:31:54 by malrifai         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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

int	resolve_cd_target(char **args, t_env **env, char **target_dir)
{
	if (!args[1])
	{
		if (no_args(env, target_dir) == 1)
			return (1);
	}
	else if (ft_strcmp(args[1], "-") == 0)
	{
		*target_dir = get_env_value(*env, "OLDPWD");
		if (!*target_dir)
		{
			printf("cd: OLDPWD not set\n");
			return (1);
		}
		printf("%s\n", *target_dir);
	}
	else
		*target_dir = args[1];
	return (0);
}

void	handle_cd(char **args, t_env **env)
{
	char	*target_dir;

	if (resolve_cd_target(args, env, &target_dir) == 1)
		return ;
	if (chdir(target_dir) == -1)
	{
		perror("cd");
		return ;
	}
	if (update_pwd_env(env) != 0)
		return ;
}
