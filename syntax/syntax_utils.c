/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntax_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: malrifai <malrifai@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/22 12:32:11 by malrifai          #+#    #+#             */
/*   Updated: 2025/04/30 15:13:51 by malrifai         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	has_invalid_operator_sequence(char *input)
{
	int	i;

	i = 0;
	while (input[i])
	{
		if (ft_strncmp(&input[i], "&&&", 3) == 0
			|| ft_strncmp(&input[i], "|||", 3) == 0
			|| ft_strncmp(&input[i], "|&&", 3) == 0
			|| ft_strncmp(&input[i], "&&|", 3) == 0)
			return (1);
		i++;
	}
	return (0);
}

int	has_unmatched_parentheses(const char *input)
{
	int	depth;
	int	i;

	depth = 0;
	i = 0;
	while (input[i])
	{
		if (input[i] == '(')
			depth++;
		else if (input[i] == ')')
		{
			if (depth == 0)
				return (1);
			depth--;
		}
		i++;
	}
	if (depth != 0)
		return (1);
	return (0);
}

int	has_empty_parentheses(char *input)
{
	int	i;

	i = 0;
	while (input[i])
	{
		if (input[i] == '(')
		{
			i++;
			while (input[i] && input[i] == ' ')
				i++;
			if (input[i] == ')')
				return (1);
		}
		i++;
	}
	return (0);
}

int	has_unmatched_quotes(char *input)
{
	char	quote;

	quote = 0;
	while (*input)
	{
		if ((*input == '\'' || *input == '"'))
		{
			if (!quote)
				quote = *input;
			else if (quote == *input)
				quote = 0;
		}
		input++;
	}
	if (quote != 0)
		return (1);
	return (0);
}

int	starts_with_pipe_or_logical(char *input)
{
	int	len;

	while (*input && *input == ' ')
		input++;
	if (ft_strncmp(input, "||", 2) == 0 || ft_strncmp(input, "&&", 2) == 0
		|| *input == '|')
		return (1);
	len = ft_strlen(input);
	if (len < 1)
		return (0);
	while (len > 0 && *input == ' ')
		len--;
	if (len >= 2 && (ft_strncmp(&input[len - 2], "||", 2) == 0
			|| ft_strncmp(&input[len - 2], "&&", 2) == 0))
		return (1);
	if (len >= 1 && input[len - 1] == '|')
		return (1);
	return (0);
}
