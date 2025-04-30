/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_wildcard.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: malrifai <malrifai@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/30 15:39:56 by malrifai          #+#    #+#             */
/*   Updated: 2025/04/30 17:47:28 by malrifai         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	add_match_to_list(
	t_token **matches_head, t_token **matches_tail, t_token *new_tok)
{
	if (!*matches_head)
	{
		*matches_head = new_tok;
		*matches_tail = new_tok;
	}
	else
	{
		(*matches_tail)->next = new_tok;
		*matches_tail = new_tok;
	}
}

static void	match_files_for_token(DIR *dir, t_token *cur,
	t_token **matches_head, t_token **matches_tail)
{
	struct dirent	*entry;
	t_token			*new_tok;

	entry = readdir(dir);
	while (entry)
	{
		if (entry->d_name[0] == '.')
		{
			entry = readdir(dir);
			continue ;
		}
		if (fnmatch(cur->value, entry->d_name, 0) == 0)
		{
			new_tok = new_token(entry->d_name, WORD);
			add_match_to_list(matches_head, matches_tail, new_tok);
		}
		entry = readdir(dir);
	}
}

static void	replace_token_with_matches(t_token *cur,
	t_token *matches_head, t_token *matches_tail)
{
	t_token	*tmp;

	free(cur->value);
	cur->value = ft_strdup(matches_head->value);
	tmp = matches_head->next;
	matches_tail->next = cur->next;
	cur->next = tmp;
	free(matches_head->value);
	free(matches_head);
}

void	expand_wildcards(t_token *tokens)
{
	DIR			*dir;
	t_token		*cur;
	t_token		*matches_head;
	t_token		*matches_tail;

	cur = tokens;
	while (cur)
	{
		if (cur->type == WORD && ft_strchr(cur->value, '*'))
		{
			dir = opendir(".");
			if (!dir)
				return ;
			matches_head = NULL;
			matches_tail = NULL;
			match_files_for_token(dir, cur, &matches_head, &matches_tail);
			closedir(dir);
			if (matches_head)
				replace_token_with_matches(cur, matches_head, matches_tail);
		}
		cur = cur->next;
	}
}
