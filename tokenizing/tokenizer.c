/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: malrifai <malrifai@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/04 22:19:03 by malrifai          #+#    #+#             */
/*   Updated: 2025/02/19 22:58:17 by malrifai         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	handle_export_tokenization(int *i, char *input, t_token **head)
{
	if (!*head && ft_strncmp(&input[*i], "export", 6) == 0
		&& (input[*i + 6] == ' ' || input[*i + 6] == '\0'))
	{
		*i += 6;
		while (input[*i] == ' ')
			(*i)++;
		add_token(head, "export", WORD);
		if (input[*i] != '\0')
			add_token(head, ft_strdup(&input[*i]), WORD);
		return (1);
	}
	return (0);
}

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
		if (handle_export_tokenization(&i, input, &head))
			break ;
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
