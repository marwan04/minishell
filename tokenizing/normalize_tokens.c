/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   normalize_tokens.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: malrifai <malrifai@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/23 17:32:35 by malrifai          #+#    #+#             */
/*   Updated: 2025/04/30 15:35:29 by malrifai         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	append_redirection(
	t_redir_state *state, t_token *redir, t_token *file)
{
	redir->next = file;
	file->next = NULL;
	if (!state->list)
	{
		state->list = redir;
		state->tail = file;
	}
	else
	{
		state->tail->next = redir;
		state->tail = file;
	}
}

void	insert_redirections_back(
	t_token **tokens, t_redir_state *state,
	t_token *prev, t_token *cur)
{
	if (prev)
		prev->next = state->list;
	else
		*tokens = state->list;
	state->tail->next = cur;
}

void	handle_redirection_case(
	t_token **tokens, t_token **cur,
	t_token **prev, t_redir_state *state)
{
	t_token	*redir;
	t_token	*file;

	redir = *cur;
	file = (*cur)->next;
	if (*prev)
		(*prev)->next = file->next;
	else
		*tokens = file->next;
	*cur = file->next;
	append_redirection(state, redir, file);
}

static void	process_normalization_loop(
	t_token **tokens, t_token **cur,
	t_token **prev, t_redir_state *state)
{
	while (*cur)
	{
		if (is_pipe_or_logical(*cur))
		{
			if (state->list)
				insert_redirections_back(tokens, state, *prev, *cur);
			state->list = NULL;
			state->tail = NULL;
			*prev = *cur;
			*cur = (*cur)->next;
		}
		else if (is_redirection(*cur)
			&& (*cur)->type != HEREDOC && (*cur)->next)
			handle_redirection_case(tokens, cur, prev, state);
		else
		{
			*prev = *cur;
			*cur = (*cur)->next;
		}
	}
}

void	normalize_tokens(t_token **tokens)
{
	t_token			*cur;
	t_token			*prev;
	t_redir_state	state;

	cur = *tokens;
	prev = NULL;
	state.list = NULL;
	state.tail = NULL;
	process_normalization_loop(tokens, &cur, &prev, &state);
	if (state.list)
		insert_redirections_back(tokens, &state, prev, NULL);
}
