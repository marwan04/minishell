/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: malrifai <malrifai@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/10 14:50:31 by malrifai          #+#    #+#             */
/*   Updated: 2025/02/15 18:53:36 by malrifai         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

char	*expand_tilde(char *token)
{
	char	*home;
	char	*expanded_token;

	if (!token || token[0] != '~')
		return (ft_strdup(token));
	home = getenv("HOME");
	if (!home)
		return (ft_strdup(token));
	if (token[1] == '\0')
		return (ft_strdup(home));
	expanded_token = ft_strjoin(home, token + 1);
	return (expanded_token);
}

char	*remove_quotes(char *input)
{
	char	*cleaned;
	int		i;
	char	quote;
	char	c[2];

	cleaned = ft_strdup("");
	i = 0;
	quote = 0;
	while (input[i])
	{
		if ((input[i] == '"' || input[i] == '\'') && !quote)
			quote = input[i];
		else if (input[i] == quote)
			quote = 0;
		else
		{
			c[0] = input[i];
			c[1] = '\0';
			cleaned = ft_strjoin_free(cleaned, c);
		}
		i++;
	}
	return (cleaned);
}

void	expand_tokens(t_token *tokens, int last_exit_status)
{
	char	*expanded;

	while (tokens)
	{
		expanded = expand_tilde(tokens->value);
		expanded = expand_variables(expanded, last_exit_status);
		expanded = remove_quotes(expanded);
		if (expanded != tokens->value)
		{
			free(tokens->value);
			tokens->value = expanded;
		}
		tokens = tokens->next;
	}
}
