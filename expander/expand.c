/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: malrifai <malrifai@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/10 14:50:31 by malrifai          #+#    #+#             */
/*   Updated: 2025/02/15 16:43:20 by malrifai         ###   ########.fr       */
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

char	*expand_variables(char *token, int last_exit_status)
{
	int			i;
	t_expand	expand;

	if (!token || !ft_strchr(token, '$'))
		return (ft_strdup(token));
	expand.expanded = ft_strdup("");
	i = 0;
	while (token[i])
	{
		if (token[i] == '\'')
		{
			expand.start = i++;
			while (token[i] && token[i] != '\'')
				i++;
			if (token[i] == '\'')
				i++;
			expand.expanded = ft_strjoin_free(expand.expanded, \
				ft_substr(token, expand.start, i - expand.start));
		}
		else if (token[i] == '$')
		{
			expand.start = ++i;
			if (!ft_isalpha(token[expand.start]) && token[expand.start]
				!= '_' && token[expand.start] != '?')
			{
				expand.expanded = ft_strjoin_free(expand.expanded, "$");
				continue ;
			}
			if (token[expand.start] == '?')
			{
				expand.exit_status = ft_itoa(last_exit_status);
				expand.expanded = ft_strjoin_free(expand.expanded, \
					expand.exit_status);
				free(expand.exit_status);
				i++;
			}
			else
			{
				while (token[i] && (ft_isalnum(token[i]) || token[i] == '_'))
					i++;
				expand.var_name = ft_substr(token, expand.start, \
					i - expand.start);
				expand.var_value = getenv(expand.var_name);
				if (expand.var_value)
					expand.expanded = ft_strjoin_free(expand.expanded, \
						expand.var_value);
				else
					expand.expanded = ft_strjoin_free(expand.expanded, "");
				free(expand.var_name);
			}
		}
		else
		{
			expand.c[0] = token[i];
			expand.c[1] = '\0';
			expand.expanded = ft_strjoin_free(expand.expanded, expand.c);
			i++;
		}
	}
	return (expand.expanded);
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
