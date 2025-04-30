/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eaqrabaw <eaqrabaw@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/10 14:50:31 by malrifai          #+#    #+#             */
/*   Updated: 2025/04/30 06:02:32 by eaqrabaw         ###   ########.fr       */
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
	int		i, j;
	char	quote;

	if (!input)
		return (NULL);

	cleaned = malloc(ft_strlen(input) + 1); // worst-case: same length
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
	if ((tokens->value[0] == '\'' && tokens->value[ft_strlen(tokens->value) - 1] == '\'')
		|| (tokens->value[0] == '"' && tokens->value[ft_strlen(tokens->value) - 1] == '"'))
		tokens->herdoc_quote = 1;
	else
		tokens->herdoc_quote = 0;
}

void expand_tokens(t_token *tokens, int last_exit_status, t_env *env)
{
	char *tilde_expanded;
	char *vars_expanded;
	char *quoted_clean;

	while (tokens)
	{
		if (!tokens->value)
		{
			tokens = tokens->next;
			continue;
		}

		tilde_expanded = expand_tilde(tokens->value);  // malloc'd
		if (!tilde_expanded)
		{
			tokens = tokens->next;
			continue;
		}

		vars_expanded = expand_variables(tilde_expanded, last_exit_status, env);  // malloc'd
		free(tilde_expanded);
		if (!vars_expanded)
		{
			tokens = tokens->next;
			continue;
		}

		quoted_clean = remove_quotes(vars_expanded);  // malloc'd
		free(vars_expanded);

		if (!quoted_clean)
		{
			tokens = tokens->next;
			continue;
		}

		free(tokens->value);                  // ✅ Always free old value
		tokens->value = quoted_clean;         // ✅ Replace with cleaned value

		if (tokens->type == HEREDOC)
			check_del_flag(tokens->next);

		tokens = tokens->next;
	}
}


void expand_wildcards(t_token *tokens)
{
	DIR *dir;
	struct dirent *entry;
	t_token *cur = tokens;
	t_token *matches_head;
	t_token *matches_tail;
	t_token *new_tok;
	t_token *tmp;
	
	while (cur)
	{
		if (cur->type == WORD && ft_strchr(cur->value, '*'))
		{
			dir = opendir(".");
			if (!dir)
				return;
			matches_head = NULL;
			matches_tail = NULL;
			while ((entry = readdir(dir)))
			{
				if (entry->d_name[0] == '.')
					continue;
				if (fnmatch(cur->value, entry->d_name, 0) == 0)
				{
					new_tok = new_token(entry->d_name, WORD);
					if (!matches_head)
					{
						matches_head = new_tok;
						matches_tail = new_tok;
					}
					else
					{
						matches_tail->next = new_tok;
						matches_tail = new_tok;
					}
				}
			}
			closedir(dir);
			if (matches_head)
			{
				free(cur->value);
				cur->value = ft_strdup(matches_head->value);
				tmp = matches_head->next;
				matches_tail->next = cur->next;
				cur->next = tmp;
				free(matches_head->value);
				free(matches_head);
			}
		}
		cur = cur->next;
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
