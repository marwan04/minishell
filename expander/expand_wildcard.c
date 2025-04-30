/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_wildcard.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: malrifai <malrifai@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/30 15:39:56 by malrifai          #+#    #+#             */
/*   Updated: 2025/04/30 15:42:01 by malrifai         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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
