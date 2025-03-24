/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eaqrabaw <eaqrabaw@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/03 10:10:15 by eaqrabaw          #+#    #+#             */
/*   Updated: 2025/03/24 09:10:57 by eaqrabaw         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./includes/minishell.h"

void	print_tokens(t_token *head)
{
	t_token	*tmp;

	tmp = head;
	while (tmp)
	{
		printf("Token: [%s] (Type: %d)\n", tmp->value, tmp->type);
		tmp = tmp->next;
	}
}

void	print_commands(t_cmd *cmds)
{
	int		i;
	char	*append;
	t_cmd	*tmp;

	tmp = cmds;
	while (tmp)
	{
		printf("Command: ");
		i = 0;
		while (tmp->args && tmp->args[i])
			printf("[%s] ", tmp->args[i++]);
		printf("\n");
		if (tmp->input)
			printf("  Input Redirection: %s\n", tmp->input);
		if (tmp->output)
		{
			if (tmp->append)
				append = "append";
			else
				append = "overwrite";
			printf("  Output Redirection: %s (%s)\n", tmp->output, append);
		}
		if (tmp->pipe)
			printf("  Pipe: Yes\n");
		printf("\n");
		tmp = tmp->next;
	}
}

void ft_exit(t_minishell *data, char *input)
{
	free(input);
	if (!data->cmds->args[1])
		data->last_exit_status = 0;
	else
		data->last_exit_status = ft_atoi(data->cmds->args[1]);
	ft_free(data, 0, "exit");	
}

void	ft_read(t_minishell *data)
{
	char	*input;

	signals_handler();
	input = readline("minishell> ");
	if (!input)
		ft_free(data, 1, "exit");
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
	if (!ft_strcmp(input, "exit"))
		ft_exit(data, input);
	//execute_builtin_cmds(data->cmds, &data->last_exit_status, &data->env);
	//execute_cmds(data->cmds, &data->last_exit_status, &data->env);
	ft_execute(data->cmds, &data->last_exit_status, &data->env);
	print_tokens(data->tokens);
	// printf("\n");
	// print_commands(data->cmds);
	// printf("You entered: %s\n", input);
	if (!ft_strcmp(input, "clear"))
		printf("\033[H\033[2J");
	free(input);
}

int	main(int ac, char **av, char **envp)
{
	t_minishell	data;

	ft_bzero(&data, sizeof(t_minishell));
	(void)av;
	if (ac != 1)
		return (1);
	data.env = init_env_list(envp);
	data.last_exit_status = 0;
	while (1)
	{
		ft_read(&data);
	}
	free_env(data.env);
	return (0);
}
