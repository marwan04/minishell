/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   var_expand.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: malrifai <malrifai@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/04 23:23:41 by malrifai          #+#    #+#             */
/*   Updated: 2025/05/04 23:29:06 by malrifai         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	handle_dollar_expansion(t_expand *exp,
	char *token, int last_exit_status, t_env *env)
{
	exp->var_name = expand_extract_var(exp, token, last_exit_status);
	if (exp->skip_env_lookup)
		exp->expanded = ft_strjoin_free(exp->expanded, exp->var_name);
	else
	{
		exp->var_value = expand_replace_var(exp->var_name,
				exp->preserve_spaces, env);
		exp->expanded = ft_strjoin_free(exp->expanded, exp->var_value);
		free(exp->var_value);
	}
	free(exp->var_name);
}

void	append_char_if_not_dollar(t_expand *exp, char c)
{
	expand_append_char(exp, c);
	exp->i++;
}

static void	handle_var_expansion(t_expand *exp, char *line, t_env *env)
{
	exp->var_name = expand_extract_var(exp, line, 0);
	if (exp->skip_env_lookup)
	{
		free(exp->expanded);
		exp->expanded = ft_strdup(exp->var_name);
	}
	else
	{
		exp->var_value = expand_replace_var(exp->var_name,
				exp->preserve_spaces, env);
		exp->expanded = ft_strjoin_free(exp->expanded, exp->var_value);
		free(exp->var_value);
	}
	free(exp->var_name);
}

char	*expand_vars(char *line, t_env *env)
{
	char		*expanded;
	t_expand	exp;

	if (!line)
		return (NULL);
	ft_bzero(&exp, sizeof(t_expand));
	exp.expanded = ft_strdup("");
	while (line[exp.i])
	{
		expand_track_quotes(&exp, line[exp.i]);
		if (line[exp.i] == '$' && exp.quote != '\'' && line[exp.i + 1])
			handle_var_expansion(&exp, line, env);
		else
			expand_append_char(&exp, line[exp.i++]);
	}
	expanded = exp.expanded;
	return (expanded);
}

char	*expand_variables(char *token, int last_exit_status, t_env *env)
{
	t_expand	exp;

	if (!token || !ft_strchr(token, '$'))
		return (ft_strdup(token));
	ft_bzero(&exp, sizeof(t_expand));
	exp.expanded = ft_strdup("");
	while (token[exp.i])
	{
		expand_track_quotes(&exp, token[exp.i]);
		if (token[exp.i] == '$' && exp.quote != '\'')
			handle_dollar_expansion(&exp, token, last_exit_status, env);
		else
			append_char_if_not_dollar(&exp, token[exp.i]);
	}
	return (exp.expanded);
}
