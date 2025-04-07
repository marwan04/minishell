/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   read.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eaqrabaw <eaqrabaw@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/07 06:46:53 by eaqrabaw          #+#    #+#             */
/*   Updated: 2025/04/07 07:34:36 by eaqrabaw         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	ft_exit(t_minishell *data)
{
	if (data->env)
		free_env(data->env);
	if (!data->cmds->args[1])
		ft_free(data, data->last_exit_status, "exit");
	if (!ft_isnumeric(data->cmds->args[1]))
	{
		ft_putstr_fd("bash: exit: ", 2);
		ft_putstr_fd(data->cmds->args[1], 2);
		ft_putendl_fd(": numeric argument required", 2);
		ft_free(data, 2, "exit");
	}
	if (data->cmds->args[2])
	{
		write(1, "exit\n", 5);
		ft_putendl_fd("bash: exit: too many arguments", 2);
		data->last_exit_status = 1;
		return ;
	}
	data->last_exit_status = ft_atoi(data->cmds->args[1]);
	ft_free(data, data->last_exit_status, "exit");
}

void	ft_handle_exit(t_minishell *data, char *input)
{
    if (!ft_strcmp(data->cmds->args[0], "exit"))
        ft_exit(data);
    if (!ft_strcmp(input, "clear"))
        printf("\033[H\033[2J");
}

void	ft_process_input(t_minishell *data, char *input)
{
    if (*input)
    {
        add_history(input);
        free_tokens(data);
        free_cmds(data);
        data->tokens = tokenizer(input);
        if (data->tokens)
        {
            expand_tokens(data->tokens, data->last_exit_status, data->env);
            data->cmds = parse_tokens(data->tokens);
        }
    }
}

void	ft_read(t_minishell *data)
{
    char	*input;

    signals_handler();
    input = readline("minishell> ");
    if (!input)
    {
        free_env(data->env);
        ft_free(data, 1, "exit");
    }
    ft_process_input(data, input);
    if (data->cmds)
    {
        ft_handle_exit(data, input);
        ft_execute(data->cmds, &data->last_exit_status, &data->env, data);
    }
    free(input);
}
