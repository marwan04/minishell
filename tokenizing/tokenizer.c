/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: malrifai <malrifai@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/04 22:19:03 by malrifai          #+#    #+#             */
/*   Updated: 2025/04/15 20:03:11 by malrifai         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	skip_whitespace(char *input, int *i)
{
	while (input[*i] == ' ' || input[*i] == '\t')
		(*i)++;
}

int	check_pipe(int *i, char *input, t_token **head)
{
	if (input[*i] == '|' && input[*i + 1] != '|')
	{
		add_token(head, "|", PIPE);
		(*i)++;
		return (1);
	}
	return (0);
}

int	check_parentheses(int *i, char *input, t_token **head)
{
	if (input[*i] == '(')
	{
		add_token(head, "(", LPAREN);
		(*i)++;
		return (1);
	}
	else if (input[*i] == ')')
	{
		add_token(head, ")", RPAREN);
		(*i)++;
		return (1);
	}
	return (0);
}

int	check_logicals(int *i, char *input, t_token **head)
{
	if (input[*i] == '&' || input[*i] == '|')
	{
		if (input[*i] == '&' && input[*i + 1] == '&')
		{
			add_token(head, "&&", AND);
			(*i) += 2;
		}
		else if (input[*i] == '|' && input[*i + 1] == '|')
		{
			add_token(head, "||", OR);
			(*i) += 2;
		}
		return (1);
	}
	return (0);
}

t_token	*tokenizer(char *input)
{
	t_token	*head;
	int		i;
	char	symbol[2];

	head = NULL;
	i = 0;
	while (input[i])
	{
		skip_whitespace(input, &i);
		if (check_pipe(&i, input, &head))
			continue ;
		if (check_parentheses(&i, input, &head))
			continue ;
		if (check_logicals(&i, input, &head))
			continue ;
		if (check_redirections(&i, input, &head, symbol))
			continue ;
		if (check_quote(&i, input, &head))
			continue ;
		if (handle_words(&i, input, &head))
			continue ;
	}
	return (head);
}
