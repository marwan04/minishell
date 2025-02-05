/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: malrifai <malrifai@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/03 10:10:15 by eaqrabaw          #+#    #+#             */
/*   Updated: 2025/02/05 23:59:27 by malrifai         ###   ########.fr       */
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

//This file will only contain the main function
int	main(void)
{
	char	*input;
	t_token	*tokens;

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
		print_tokens(tokens);
		free(input);
	}
	return (0);
}
