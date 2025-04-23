/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   normalize_tokens.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: malrifai <malrifai@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/23 17:32:35 by malrifai          #+#    #+#             */
/*   Updated: 2025/04/23 17:43:40 by malrifai         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "minishell.h"

static void extract_redirection_pairs(
	t_token **tokens, t_token **redir_start, t_token **redir_end)
{
	t_token *cur;
	t_token *prev;
    t_token *pair;

    cur = *tokens;
    prev = NULL;
	while (cur && cur->next)
	{
		if (cur->type == REDIR_IN || cur->type == REDIR_OUT
			|| cur->type == APPEND || cur->type == HEREDOC)
		{
			pair = cur->next;
			if (prev)
				prev->next = pair->next;
			else
				*tokens = pair->next;
			cur->next = pair;
			pair->next = NULL;
			if (!*redir_start)
				*redir_start = cur;
			else
				(*redir_end)->next = cur;
			*redir_end = pair;
            if (prev)
                cur = prev->next;
            else 
			    cur = *tokens;
			continue ;
		}
		prev = cur;
		cur = cur->next;
	}
}

static void append_redirections_to_end(t_token **tokens, t_token *redir_start)
{
	t_token *tail;

	if (!redir_start)
		return ;
	if (!*tokens)
	{
		*tokens = redir_start;
		return ;
	}
	tail = *tokens;
	while (tail->next)
		tail = tail->next;
	tail->next = redir_start;
}

void normalize_redirections(t_token **tokens)
{
	t_token *redir_start = NULL;
	t_token *redir_end = NULL;

    redir_start = NULL;
    redir_end = NULL;
	extract_redirection_pairs(tokens, &redir_start, &redir_end);
	append_redirections_to_end(tokens, redir_start);
}
