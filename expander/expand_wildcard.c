/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_wildcard.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: malrifai <malrifai@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/30 15:39:56 by malrifai          #+#    #+#             */
/*   Updated: 2025/04/30 17:21:57 by malrifai         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// static void	append_match(t_token **head, t_token **tail, char *filename)
// {
// 	t_token	*new_tok;

// 	new_tok = new_token(filename, WORD);
// 	if (!*head)
// 	{
// 		*head = new_tok;
// 		*tail = new_tok;
// 	}
// 	else
// 	{
// 		(*tail)->next = new_tok;
// 		*tail = new_tok;
// 	}
// }

// static void	replace_token_with_matches(t_token *cur,
// 	t_token *matches_head, t_token *matches_tail)
// {
// 	t_token	*tmp;

// 	free(cur->value);
// 	cur->value = ft_strdup(matches_head->value);
// 	tmp = matches_head->next;
// 	matches_tail->next = cur->next;
// 	cur->next = tmp;
// 	free(matches_head->value);
// 	free(matches_head);
// }

// static void	handle_wildcard_for_token(t_token *cur)
// {
// 	DIR				*dir;
// 	struct dirent	*entry;
// 	t_token			*matches_head;
// 	t_token			*matches_tail;

// 	dir = opendir(".");
// 	if (!dir)
// 		return ;
// 	matches_head = NULL;
// 	matches_tail = NULL;
// 	entry = readdir(dir);
// 	while (entry)
// 	{
// 		if (entry->d_name[0] == '.')
// 			continue ;
// 		if (fnmatch(cur->value, entry->d_name, 0) == 0)
// 			append_match(&matches_head, &matches_tail, entry->d_name);
// 		entry = readdir(dir);
// 	}
// 	closedir(dir);
// 	if (matches_head)
// 		replace_token_with_matches(cur, matches_head, matches_tail);
// }

// void	expand_wildcards(t_token *tokens)
// {
// 	t_token	*cur;

// 	cur = tokens;
// 	while (cur)
// 	{
// 		if (cur->type == WORD && ft_strchr(cur->value, '*'))
// 			handle_wildcard_for_token(cur);
// 		cur = cur->next;
// 	}
// }

void expand_wildcards(t_token *tokens)
{
	DIR *dir;
	struct dirent *entry;
	t_token *cur;
	t_token *matches_head;
	t_token *matches_tail;
	t_token *new_tok;
	t_token *tmp;
	
    cur = tokens;
	while (cur)
	{
		if (cur->type == WORD && ft_strchr(cur->value, '*'))
		{
			dir = opendir(".");
			if (!dir)
				return;
			matches_head = NULL;
			matches_tail = NULL;
			while ((entry = readdir(dir)))
			{
				if (entry->d_name[0] == '.')
					continue;
				if (fnmatch(cur->value, entry->d_name, 0) == 0)
				{
					new_tok = new_token(entry->d_name, WORD);
					if (!matches_head)
					{
						matches_head = new_tok;
						matches_tail = new_tok;
					}
					else
					{
						matches_tail->next = new_tok;
						matches_tail = new_tok;
					}
				}
			}
			closedir(dir);
			if (matches_head)
			{
				free(cur->value);
				cur->value = ft_strdup(matches_head->value);
				tmp = matches_head->next;
				matches_tail->next = cur->next;
				cur->next = tmp;
				free(matches_head->value);
				free(matches_head);
			}
		}
		cur = cur->next;
	}
}
