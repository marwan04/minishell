/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: malrifai <malrifai@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/10 17:59:26 by malrifai          #+#    #+#             */
/*   Updated: 2025/04/30 18:59:09 by malrifai         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*ft_strjoin_free(char *s1, char *s2)
{
	char	*joined;

	joined = ft_strjoin(s1, s2);
	if (!joined)
	{
		return (NULL);
	}
	free(s1);
	return (joined);
}

void	expand_one_token(t_token *tok, int last_exit_status, t_env *env)
{
	char	*tilde_expanded;
	char	*vars_expanded;
	char	*quoted_clean;

	if (!tok->value)
		return ;
	tilde_expanded = expand_tilde(tok->value);
	if (!tilde_expanded)
		return ;
	vars_expanded = expand_variables(tilde_expanded, last_exit_status, env);
	free(tilde_expanded);
	if (!vars_expanded)
		return ;
	quoted_clean = remove_quotes(vars_expanded);
	free(vars_expanded);
	if (!quoted_clean)
		return ;
	free(tok->value);
	tok->value = quoted_clean;
}
