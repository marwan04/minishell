/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: malrifai <malrifai@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/03 10:10:15 by eaqrabaw          #+#    #+#             */
/*   Updated: 2025/02/08 16:50:29 by malrifai         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./includes/minishell.h"

void	print_tokens(t_token *head)
{
	while (head)
	{
		printf("Token: [%s] (Type: %d)\n", head->value, head->type);
		head = head->next;
	}
}

void	print_commands(t_cmd *cmds)
{
	int		i;
	char	*append;

	while (cmds)
	{
		printf("Command: ");
		i = 0;
		while (cmds->args && cmds->args[i])
			printf("[%s] ", cmds->args[i++]);
		printf("\n");
		if (cmds->input)
			printf("  Input Redirection: %s\n", cmds->input);
		if (cmds->output)
		{
			if (cmds->append)
				append = "append";
			else
				append = "overwrite";
			printf("  Output Redirection: %s (%s)\n", cmds->output, append);
		}
		if (cmds->pipe)
			printf("  Pipe: Yes\n");
		printf("\n");
		cmds = cmds->next;
	}
}

// This file will only contain the main function
int	main(void)
{
	char	*input;
	t_token	*tokens;
	t_cmd	*cmds;

	while (1)
	{
		input = readline("minishell> ");
		if (!input)
			break ;
		if (*input)
		{
			add_history(input);
			tokens = tokenizer(input);
		}
		printf("You entered: %s\n", input);
		cmds = parse_tokens(tokens);
		print_tokens(tokens);
		printf("\n");
		print_commands(cmds);
		free(input);
	}
	return (0);
}
