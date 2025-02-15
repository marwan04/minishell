/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   var_expand.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: malrifai <malrifai@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/15 18:51:48 by malrifai          #+#    #+#             */
/*   Updated: 2025/02/15 19:59:00 by malrifai         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	expand_variables4(t_expand *expand, char *token)
{
	expand->var_name = ft_substr(token, expand->start, \
		expand->i - expand->start);
	expand->var_value = getenv(expand->var_name);
	if (expand->var_value)
		expand->expanded = ft_strjoin_free(expand->expanded, expand->var_value);
	else
		expand->expanded = ft_strjoin_free(expand->expanded, "");
	free(expand->var_name);
}

int	expand_variables3(t_expand *expand, char *token, int last_exit_status)
{
	if (!ft_isalpha(token[expand->start]) && token[expand->start]
		!= '_' && token[expand->start] != '?')
	{
		expand->expanded = ft_strjoin_free(expand->expanded, "$");
		return (1);
	}
	if (token[expand->start] == '?')
	{
		expand->exit_status = ft_itoa(last_exit_status);
		expand->expanded = ft_strjoin_free(expand->expanded, \
			expand->exit_status);
		free(expand->exit_status);
		expand->i++;
	}
	else
	{
		while (token[expand->i] && \
			(ft_isalnum(token[expand->i]) || token[expand->i] == '_'))
			expand->i++;
		expand_variables4(expand, token);
	}
	return (0);
}

int	expand_variables2(t_expand *expand, char *token, int last_exit_status)
{
	if (expand->quote == '\'' || token[expand->i] == '\'')
	{
		expand->c[0] = token[expand->i];
		expand->c[1] = '\0';
		expand->expanded = ft_strjoin_free(expand->expanded, expand->c);
		expand->i++;
	}
	else if (token[expand->i] == '$' && expand->quote != '\'')
	{
		expand->start = ++expand->i;
		if (expand_variables3(expand, token, last_exit_status))
			return (1);
	}
	else
	{
		expand->c[0] = token[expand->i];
		expand->c[1] = '\0';
		expand->expanded = ft_strjoin_free(expand->expanded, expand->c);
		expand->i++;
	}
	return (0);
}

char	*expand_variables(char *token, int last_exit_status)
{
	t_expand	expand;

	if (!token || !ft_strchr(token, '$'))
		return (ft_strdup(token));
	expand.quote = 0;
	expand.expanded = ft_strdup("");
	expand.i = 0;
	while (token[expand.i])
	{
		if ((token[expand.i] == '"' || token[expand.i] == '\'') \
			&& expand.quote == 0)
			expand.quote = token[expand.i];
		else if (token[expand.i] == expand.quote)
			expand.quote = 0;
		if (expand_variables2(&expand, token, last_exit_status))
			continue ;
	}
	return (expand.expanded);
}

// char	*expand_variables(char *token, int last_exit_status)
// {
// 	int			i;
// 	t_expand	expand;

// 	if (!token || !ft_strchr(token, '$'))
// 		return (ft_strdup(token));
//     expand.quote = 0;
// 	expand.expanded = ft_strdup("");
// 	i = 0;
// 	while (token[i])
// 	{
// 		if ((token[i] == '"' || token[i] == '\'') && expand.quote == 0)
// 			expand.quote = token[i];
// 		else if (token[i] == expand.quote)
// 			expand.quote = 0;
// 		if (expand.quote == '\'' || token[i] == '\'')
// 		{
// 			expand.c[0] = token[i];
// 			expand.c[1] = '\0';
// 			expand.expanded = ft_strjoin_free(expand.expanded, expand.c);
// 			i++;
// 		}
// 		else if (token[i] == '$' && expand.quote != '\'')
// 		{
// 			expand.start = ++i;
// 			if (!ft_isalpha(token[expand.start]) && token[expand.start]
// 				!= '_' && token[expand.start] != '?')
// 			{
// 				expand.expanded = ft_strjoin_free(expand.expanded, "$");
// 				continue;
// 			}
// 			if (token[expand.start] == '?')
// 			{
// 				expand.exit_status = ft_itoa(last_exit_status);
// 				expand.expanded = ft_strjoin_free(expand.expanded, /
// 					expand.exit_status);
// 				free(expand.exit_status);
// 				i++;
// 			}
// 			else
// 			{
// 				while (token[i] && (ft_isalnum(token[i]) || token[i] == '_'))
// 					i++;
// 				expand.var_name = ft_substr(token, expand.start, /
//					i - expand.start);
// 				expand.var_value = getenv(expand.var_name);
// 				if (expand.var_value)
// 					expand.expanded = ft_strjoin_free(expand.expanded, /
//						expand.var_value);
// 				else
// 					expand.expanded = ft_strjoin_free(expand.expanded, "");
// 				free(expand.var_name);
// 			}
// 		}
// 		else
// 		{
// 			expand.c[0] = token[i];
// 			expand.c[1] = '\0';
// 			expand.expanded = ft_strjoin_free(expand.expanded, expand.c);
// 			i++;
// 		}
// 	}
// 	return (expand.expanded);
// }