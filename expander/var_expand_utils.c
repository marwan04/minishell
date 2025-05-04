/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   var_expand_utils.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: malrifai <malrifai@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/15 18:51:48 by malrifai          #+#    #+#             */
/*   Updated: 2025/05/04 23:27:46 by malrifai         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	expand_track_quotes(t_expand *expand, char c)
{
	if ((c == '"' || c == '\'') && expand->quote == 0)
	{
		expand->quote = c;
		if (expand->quote == '"')
			expand->preserve_spaces = 1;
	}
	else if (c == expand->quote)
	{
		expand->quote = 0;
		expand->preserve_spaces = 0;
	}
}

void	expand_append_char(t_expand *expand, char c)
{
	expand->c[0] = c;
	expand->c[1] = '\0';
	expand->expanded = ft_strjoin_free(expand->expanded, expand->c);
}
