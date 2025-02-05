/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alrfa3i <alrfa3i@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/04 22:19:03 by malrifai          #+#    #+#             */
/*   Updated: 2025/02/06 01:55:16 by alrfa3i          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

t_token	*tokenizer(char *input)
{
	t_token	*head;
	int		i;
	char	symbol[2];

	head = NULL;
	i = 0;
	while (input[i])
	{
		while (input[i] == ' ' || input[i] == '\t')
			i++;
		if (input[i] == '|')
		{
			add_token(&head, "|", PIPE);
			i++;
			continue ;
		}
		if (check_redirections(&i, input, &head, symbol))
			continue ;
		if (check_quote(&i, input, &head))
			continue ;
		if (handle_words(&i, input, &head))
			continue ;
	}
	return (head);
}
