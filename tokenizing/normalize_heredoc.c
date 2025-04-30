/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   normalize_heredoc.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: malrifai <malrifai@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/30 14:52:57 by malrifai          #+#    #+#             */
/*   Updated: 2025/04/30 15:45:15 by malrifai         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	append_heredoc(t_heredoc_state *state,
	t_token *heredoc, t_token *limiter)
{
	heredoc->next = limiter;
	limiter->next = NULL;
	if (!state->list)
	{
		state->list = heredoc;
		state->tail = limiter;
	}
	else
	{
		state->tail->next = heredoc;
		state->tail = limiter;
	}
}

static void	insert_heredocs_back(t_token **tokens,
	t_heredoc_state *state, t_token *prev, t_token *cur)
{
	if (prev)
		prev->next = state->list;
	else
		*tokens = state->list;
	state->tail->next = cur;
}

static void	handle_heredoc_case(t_token **tokens,
	t_token **cur, t_token **prev, t_heredoc_state *state)
{
	t_token	*heredoc;
	t_token	*limiter;

	heredoc = *cur;
	limiter = heredoc->next;
	if (*prev)
		(*prev)->next = limiter->next;
	else
		*tokens = limiter->next;
	*cur = limiter->next;
	append_heredoc(state, heredoc, limiter);
}

static void	process_heredoc_normalization(t_token **tokens,
	t_token **cur, t_token **prev, t_heredoc_state *state)
{
	while (*cur)
	{
		if (is_pipe_or_logical_or_paren(*cur))
		{
			if (state->list)
				insert_heredocs_back(tokens, state, *prev, *cur);
			state->list = NULL;
			state->tail = NULL;
			*prev = *cur;
			*cur = (*cur)->next;
		}
		else if ((*cur)->type == HEREDOC)
			handle_heredoc_case(tokens, cur, prev, state);
		else
		{
			*prev = *cur;
			*cur = (*cur)->next;
		}
	}
}

void	normalize_tokens_with_heredoc(t_token **tokens)
{
	t_token				*cur;
	t_token				*prev;
	t_heredoc_state		state;

	cur = *tokens;
	prev = NULL;
	state.list = NULL;
	state.tail = NULL;
	process_heredoc_normalization(tokens, &cur, &prev, &state);
	if (state.list)
		insert_heredocs_back(tokens, &state, prev, NULL);
}
