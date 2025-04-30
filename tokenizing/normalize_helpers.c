/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   normalize_helpers.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: malrifai <malrifai@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/30 14:44:09 by malrifai          #+#    #+#             */
/*   Updated: 2025/04/30 14:51:04 by malrifai         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_pipe_or_logical(t_token *token)
{
	if (!token)
		return (0);
	return (token->type == PIPE || token->type == AND || token->type == OR);
}

int	is_redirection(t_token *token)
{
	if (!token)
		return (0);
	return (token->type == REDIR_IN || token->type == REDIR_OUT
		|| token->type == APPEND || token->type == HEREDOC);
}

int	is_pipe_or_logical_or_paren(t_token *token)
{
	if (!token)
		return (0);
	return (token->type == PIPE || token->type == AND || token->type == OR
		|| token->type == LPAREN || token->type == RPAREN);
}
