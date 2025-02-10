/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: malrifai <malrifai@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/03 10:10:15 by eaqrabaw          #+#    #+#             */
/*   Updated: 2025/02/10 16:48:58 by malrifai         ###   ########.fr       */
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

void	ft_read(t_minishell *data)
{
	char	*input;

	input = readline("minishell> ");
	if (!input)
		ft_free(data, 1, "Couldn't Read Input");
	if (*input)
	{
		add_history(input);
		free_tokens(data);
		free_cmds(data);
		data->tokens = tokenizer(input);
		if (data->tokens)
		{
			expand_tokens(data->tokens);
			data->cmds = parse_tokens(data->tokens);
		}
	}
	if (ft_strcmp(input, "exit") == 0)
	{
		free(input);
		ft_free(data, 0, "");
	}
	printf("You entered: %s\n", input);
	print_tokens(data->tokens);
	printf("\n");
	print_commands(data->cmds);
	free(input);
}

int	main(int ac, char **av)
{
	t_minishell	data;

	ft_bzero(&data, sizeof(t_minishell));
	(void)av;
	if (ac != 1)
		return (1);
	while (1)
	{
		ft_read(&data);
	}
	return (0);
}
