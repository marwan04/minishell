/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alrfa3i <alrfa3i@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/06 00:47:37 by alrfa3i           #+#    #+#             */
/*   Updated: 2025/02/18 00:52:03 by alrfa3i          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	check_separator(int *i, char *input)
{
	if (input[*i] != '\0' && input[*i] != ' ' && input[*i] != '|'
		&& input[*i] != '<' && input[*i] != '>')
	{
		while (input[*i] && input[*i] != ' ' && input[*i] != '|'
			&& input[*i] != '<' && input[*i] != '>')
			(*i)++;
	}
}

int	check_redirections(int *i, char *input, t_token **head, char symbol[2])
{
	if (input[*i] == '>' || input[*i] == '<')
	{
		if (input[*i] == '>' && input[*i + 1] == '>')
		{
			add_token(head, ">>", APPEND);
			(*i) += 2;
		}
		else if (input[*i] == '<' && input[*i + 1] == '<')
		{
			add_token(head, "<<", HEREDOC);
			(*i) += 2;
		}
		else
		{
			symbol[0] = input[*i];
			symbol[1] = '\0';
			if (input[*i] == '>')
				add_token(head, symbol, REDIR_OUT);
			else
				add_token(head, symbol, REDIR_IN);
			(*i)++;
		}
		return (1);
	}
	return (0);
}

int	check_quote(int *i, char *input, t_token **head)
{
	char	quote;
	int		start;
	char	*quoted_str;

	if (input[*i] == '"' || input[*i] == '\'')
	{
		quote = input[*i];
		start = (*i);
		(*i)++;
		while (input[*i] && input[*i] != quote)
			(*i)++;
		if (input[*i] == quote)
		{
			(*i)++;
			check_separator(i, input);
		}
		quoted_str = ft_substr(input, start, *i - start);
		add_token(head, quoted_str, WORD);
		free(quoted_str);
		return (1);
	}
	return (0);
}

int	handle_words(int *i, char *input, t_token **head)
{
	int		start;
	char	*word;

	if (input[*i] != '\0' && input[*i] != ' ' && input[*i] != '|'
		&& input[*i] != '<' && input[*i] != '>')
	{
		start = *i;
		while (input[*i] && input[*i] != ' ' && input[*i] != '|'
			&& input[*i] != '<' && input[*i] != '>')
			(*i)++;
		word = ft_substr(input, start, *i - start);
		add_token(head, word, WORD);
		free(word);
		return (1);
	}
	return (0);
}
