/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alrfa3i <alrfa3i@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/03 10:10:15 by eaqrabaw          #+#    #+#             */
/*   Updated: 2025/04/27 15:48:05 by alrfa3i          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./includes/minishell.h"

void update_shlvl(t_env **env)
{
	t_env *var;
	int shlvl;
	char *shlvl_str;

	var = *env;
	while (var)
	{
		if (ft_strcmp(var->key, "SHLVL") == 0)
		{
			shlvl = ft_atoi(var->value);
			if (shlvl < 0)
				shlvl = 0;
			else if (shlvl >= 999)
				shlvl = 1;
			else
				shlvl += 1;
			free(var->value);
			shlvl_str = ft_itoa(shlvl);
			var->value = shlvl_str;
			return ;
		}
		var = var->next;
	}
	add_or_update_env(env, "SHLVL", "1");
}

int	main(int ac, char **av, char **envp)
{
	t_minishell	data;

	ft_bzero(&data, sizeof(t_minishell));
	(void)av;
	if (ac != 1)
		return (1);
	data.env = init_env_list(envp);
	update_shlvl(&data.env);
	if (!isatty(1) || !isatty(0))
		ft_free(&data, 0, "");
	data.last_exit_status = 0;
	init_signals();
	while (1)
	{
		ft_read(&data);
	}
	return (0);
}
