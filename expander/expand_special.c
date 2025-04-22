/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_special.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: malrifai <malrifai@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/12 17:29:23 by malrifai          #+#    #+#             */
/*   Updated: 2025/04/15 19:18:52 by malrifai         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*handle_exit_and_shell_name(t_expand *expand,
			char *token, int last_exit_status)
{
	if (token[expand->start] == '?')
	{
		expand->exit_status = ft_itoa(last_exit_status);
		expand->i++;
		expand->skip_env_lookup = 1;
		return (expand->exit_status);
	}
	if (token[expand->start] == '0')
	{
		expand->i++;
		expand->skip_env_lookup = 1;
		return (ft_strdup("minishell"));
	}
	return (NULL);
}

char	*handle_numeric_vars(t_expand *expand, char *token)
{
	if (ft_isdigit(token[expand->start])
		&& token[expand->start] != '0')
	{
		expand->start++;
		expand->i = expand->start;
		while (token[expand->i] && (ft_isalnum(token[expand->i])
				|| token[expand->i] == '_'))
			expand->i++;
		expand->skip_env_lookup = 1;
		if (expand->i > expand->start)
			return (ft_substr(token,
					expand->start,
					expand->i - expand->start));
		else
			return (ft_strdup(""));
	}
	return (NULL);
}

char	*extract_normal_var(t_expand *expand, char *token)
{
	while (token[expand->i] && (ft_isalnum(token[expand->i])
			|| token[expand->i] == '_'))
		expand->i++;
	return (ft_substr(token, expand->start, expand->i - expand->start));
}

char	*expand_extract_var(t_expand *expand, char *token, int last_exit_status)
{
	char	*result;

	expand->start = ++expand->i;
	expand->skip_env_lookup = 0;
	result = handle_exit_and_shell_name(expand, token, last_exit_status);
	if (result != NULL)
		return (result);
	result = handle_numeric_vars(expand, token);
	if (result != NULL)
		return (result);
	return (extract_normal_var(expand, token));
}
