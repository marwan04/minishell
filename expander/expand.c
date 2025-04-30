/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: malrifai <malrifai@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/10 14:50:31 by malrifai          #+#    #+#             */
/*   Updated: 2025/04/30 17:11:41 by malrifai         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*expand_tilde(char *token)
{
	char	*home;
	char	*expanded_token;

	if (!token)
		return (NULL);
	if (token[0] != '~')
		return (ft_strdup(token));
	home = getenv("HOME");
	if (!home)
		return (ft_strdup(token));
	if (token[1] == '\0')
		return (ft_strdup(home));
	expanded_token = ft_strjoin(home, token + 1);
	if (!expanded_token)
		return (NULL);
	return (expanded_token);
}

char	*remove_quotes(char *input)
{
	char	*cleaned;
	int		i;
	int		j;
	char	quote;

	if (!input)
		return (NULL);
	cleaned = malloc(ft_strlen(input) + 1);
	if (!cleaned)
		return (NULL);
	i = 0;
	j = 0;
	quote = 0;
	while (input[i])
	{
		if ((input[i] == '"' || input[i] == '\'') && !quote)
			quote = input[i];
		else if (input[i] == quote)
			quote = 0;
		else
			cleaned[j++] = input[i];
		i++;
	}
	cleaned[j] = '\0';
	return (cleaned);
}

void	check_del_flag(t_token *tokens)
{
	if ((tokens->value[0] == '\''
			&& tokens->value[ft_strlen(tokens->value) - 1] == '\'')
		|| (tokens->value[0] == '"'
			&& tokens->value[ft_strlen(tokens->value) - 1] == '"'))
		tokens->herdoc_quote = 1;
	else
		tokens->herdoc_quote = 0;
}

void	expand_tokens(t_token *tokens, int last_exit_status, t_env *env)
{
	while (tokens)
	{
		expand_one_token(tokens, last_exit_status, env);
		if (tokens->type == HEREDOC)
			check_del_flag(tokens->next);
		tokens = tokens->next;
	}
}

char	*expand_line(char *line, t_minishell *data)
{
	char	*tilde_expanded;
	char	*vars_expanded;
	char	*final;

	if (!line)
		return (NULL);
	tilde_expanded = expand_tilde(line);
	if (!tilde_expanded)
		return (NULL);
	vars_expanded = expand_variables(tilde_expanded,
			data->last_exit_status, data->env);
	free(tilde_expanded);
	if (!vars_expanded)
		return (NULL);
	final = remove_quotes(vars_expanded);
	free(vars_expanded);
	return (final);
}
