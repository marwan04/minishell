/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntax_check.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: malrifai <malrifai@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/22 15:32:46 by malrifai          #+#    #+#             */
/*   Updated: 2025/04/22 15:54:39 by malrifai         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	validate_token_sequence(t_token *tokens)
{
	t_token	*cur;

	cur = tokens;
	while (cur)
	{
		if ((cur->type == AND || cur->type == OR || cur->type == PIPE)
			&& cur->next
			&& (cur->next->type == AND || cur->next->type == OR
				|| cur->next->type == PIPE))
			return (1);
		if ((cur->type == REDIR_IN || cur->type == REDIR_OUT
				|| cur->type == APPEND || cur->type == HEREDOC)
			&& (!cur->next || cur->next->type != WORD))
			return (1);
		if ((cur->type == REDIR_IN || cur->type == REDIR_OUT
				|| cur->type == APPEND || cur->type == HEREDOC)
			&& cur->next && (cur->next->type == REDIR_IN
				|| cur->next->type == REDIR_OUT
				|| cur->next->type == APPEND || cur->next->type == HEREDOC))
			return (1);
		cur = cur->next;
	}
	return (0);
}

int	syntax_error(char *input)
{
	if (starts_with_pipe_or_logical(input))
		return (1);
	if (has_unmatched_quotes(input))
		return (1);
	if (has_empty_parentheses(input))
		return (1);
	if (has_invalid_operator_sequence(input))
		return (1);
	if (has_unmatched_parentheses(input))
		return (1);
	return (0);
}
