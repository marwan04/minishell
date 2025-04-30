/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: malrifai <malrifai@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/06 01:01:38 by malrifai          #+#    #+#             */
/*   Updated: 2025/04/30 14:24:53 by malrifai         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_token	*new_token(char *value, t_token_type type)
{
	t_token	*token;

	if (!value)
		return (NULL);
	token = (t_token *)malloc(sizeof(t_token));
	if (!token)
		return (NULL);
	token->value = ft_strdup(value);
	if (!token->value)
	{
		free(token);
		return (NULL);
	}
	token->type = type;
	token->next = NULL;
	return (token);
}

t_token	*last_token(t_token *token)
{
	while (token && token->next)
		token = token->next;
	return (token);
}

void	add_token(t_token **head, char *value, t_token_type type)
{
	t_token	*new;
	t_token	*tmp;

	if (!value)
		return ;
	new = new_token(value, type);
	if (!new)
	{
		return ;
	}
	if (!*head)
	{
		*head = new;
		return ;
	}
	tmp = *head;
	while (tmp->next)
		tmp = tmp->next;
	tmp->next = new;
}

void	remove_last_token(t_token **head)
{
	t_token	*tmp;
	t_token	*prev;

	if (!head || !*head)
		return ;
	tmp = *head;
	prev = NULL;
	while (tmp->next)
	{
		prev = tmp;
		tmp = tmp->next;
	}
	if (!prev)
	{
		free(tmp->value);
		free(tmp);
		*head = NULL;
	}
	else
	{
		prev->next = NULL;
		free(tmp->value);
		free(tmp);
	}
}

void	delete_next_token(t_token *prev_token)
{
	t_token	*to_free;

	if (!prev_token || !prev_token->next)
		return ;
	to_free = prev_token->next;
	prev_token->next = prev_token->next->next;
	free(to_free->value);
	free(to_free);
}
