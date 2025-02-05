/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alrfa3i <alrfa3i@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/06 01:01:38 by alrfa3i           #+#    #+#             */
/*   Updated: 2025/02/06 01:47:50 by alrfa3i          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

t_token	*new_token(char *value, t_token_type type)
{
	t_token	*token;

	token = malloc(sizeof(t_token));
	if (!token)
		return (NULL);
	token->value = ft_strdup(value);
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
	t_token	*tmp;
	t_token	*new;

	new = new_token(value, type);
	if (!new)
		return ;
	if (!*head)
		*head = new;
	else
	{
		tmp = last_token(*head);
		tmp->next = new;
	}
}
