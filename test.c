/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eaqrabaw <eaqrabaw@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/07 06:48:42 by eaqrabaw          #+#    #+#             */
/*   Updated: 2025/04/07 06:48:53 by eaqrabaw         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./includes/minishell.h"

void	print_tokens(t_token *head)
{
	t_token	*tmp;

	// THIS FUNCTION IS FOR TESTING ONLY
	tmp = head;
	while (tmp)
	{
		printf("Token: [%s] (Type: %d)\n", tmp->value, tmp->type);
		tmp = tmp->next;
	}
}

void	print_commands(t_cmd *cmds)
{
	int		i;
	char	*append;
	t_cmd	*tmp;
	// THIS FUNCTION IS FOR TESTING ONLY
	tmp = cmds;
	while (tmp)
	{
		printf("Command: ");
		i = 0;
		while (tmp->args && tmp->args[i])
			printf("[%s] ", tmp->args[i++]);
		printf("\n");
		if (tmp->input)
			printf("  Input Redirection: %s\n", tmp->input);
		if (tmp->output)
		{
			if (tmp->append)
				append = "append";
			else
				append = "overwrite";
			printf("  Output Redirection: %s (%s)\n", tmp->output, append);
		}
		if (tmp->pipe)
			printf("  Pipe: Yes\n");
		printf("\n");
		tmp = tmp->next;
	}
}