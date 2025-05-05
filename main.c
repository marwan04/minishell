/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eaqrabaw <eaqrabaw@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/03 10:10:15 by eaqrabaw          #+#    #+#             */
/*   Updated: 2025/05/05 06:28:56 by eaqrabaw         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./includes/minishell.h"

void	update_shlvl(t_env **env)
{
	t_env	*var;
	int		shlvl;
	char	*new_val;

	var = *env;
	while (var)
	{
		if (ft_strcmp(var->key, "SHLVL") == 0)
		{
			shlvl = ft_atoi(var->value) + 1;
			new_val = ft_itoa(shlvl);
			free(var->value);
			var->value = new_val;
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
