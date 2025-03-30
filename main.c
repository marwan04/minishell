/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: malrifai <malrifai@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/03 10:10:15 by eaqrabaw          #+#    #+#             */
/*   Updated: 2025/03/30 13:58:39 by malrifai         ###   ########.fr       */
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

void	ft_exit(t_minishell *data)
{
	if (!data->cmds->args[1])
		ft_free(data, data->last_exit_status, "exit");
	if (!ft_isnumeric(data->cmds->args[1]))
	{
		ft_putstr_fd("bash: exit: ", 2);
		ft_putstr_fd(data->cmds->args[1], 2);
		ft_putendl_fd(": numeric argument required", 2);
		ft_free(data, 255, "exit");
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
	if (!ft_strcmp(data->cmds->args[0], "exit"))
		ft_exit(data);
	// execute_builtin_cmds(data->cmds, &data->last_exit_status, &data->env);
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
