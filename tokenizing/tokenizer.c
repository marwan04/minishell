/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: malrifai <malrifai@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/04 22:19:03 by malrifai          #+#    #+#             */
/*   Updated: 2025/02/08 14:04:12 by malrifai         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

t_token	*tokenizer(char *input)
{
	t_token	*head;
	int		i;
	char	symbol[2];

	head = NULL;
	i = 0;
	while (input[i])
	{
		while (input[i] == ' ' || input[i] == '\t')
			i++;
		if (input[i] == '|')
		{
			add_token(&head, "|", PIPE);
			i++;
			continue ;
		}
		if (check_redirections(&i, input, &head, symbol))
			continue ;
		if (check_quote(&i, input, &head))
			continue ;
		if (handle_words(&i, input, &head))
			continue ;
	}
	return (head);
}

t_cmd	*parse_tokens(t_token *tokens)
{
	t_cmd	*head;
	t_cmd	*current;

	head = NULL;
	current = NULL;
	while (tokens)
	{
		if (!head)
		{
			head = new_cmd();
			current = head;
		}
		else
		{
			current->next = new_cmd();
			current = current->next;
		}
		while (tokens && tokens->type == WORD)
		{
			add_argument(current, tokens->value);
			tokens = tokens->next;
		}
		if (tokens && (tokens->type == REDIR_IN || tokens->type == REDIR_OUT
				|| tokens->type == APPEND))
		{
			handle_redirection(current, tokens);
			tokens = tokens->next->next;
		}
		if (tokens && tokens->type == PIPE)
		{
			current->pipe = 1;
			tokens = tokens->next;
		}
	}
	return (head);
}
