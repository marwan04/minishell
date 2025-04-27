/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   normalize_tokens.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: malrifai <malrifai@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/23 17:32:35 by malrifai          #+#    #+#             */
/*   Updated: 2025/04/27 19:10:46 by malrifai         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "minishell.h"

int is_pipe_or_logical(t_token *token)
{
	if (!token)
		return 0;
	return (token->type == PIPE || token->type == AND || token->type == OR);
}

int is_redirection(t_token *token)
{
	if (!token)
		return 0;
	return (token->type == REDIR_IN || token->type == REDIR_OUT || token->type == APPEND || token->type == HEREDOC);
}

void	normalize_tokens(t_token **tokens)
{
	t_token *cur;
	t_token *prev;
	t_token *redir_list;
	t_token *redir_tail;

	cur = *tokens;
	prev = NULL;
	redir_list = NULL;
	redir_tail = NULL;
	while (cur)
	{
		if (is_pipe_or_logical(cur))
		{
			if (redir_list)
			{
				if (prev)
					prev->next = redir_list;
				else
					*tokens = redir_list;
				
				redir_tail->next = cur;
				redir_list = NULL;
				redir_tail = NULL;
			}
			prev = cur;
			cur = cur->next;
		}
		else if (is_redirection(cur))
		{
			t_token *redir = cur;
			t_token *file = cur->next;
			if (prev)
				prev->next = file->next;
			else
				*tokens = file->next;
			cur = file->next;
			redir->next = file;
			file->next = NULL;
			if (!redir_list)
			{
				redir_list = redir;
				redir_tail = file;
			}
			else
			{
				redir_tail->next = redir;
				redir_tail = file;
			}
		}
		else
		{
			prev = cur;
			cur = cur->next;
		}
	}
	if (redir_list)
	{
		if (prev)
			prev->next = redir_list;
		else
			*tokens = redir_list;
	}
}
