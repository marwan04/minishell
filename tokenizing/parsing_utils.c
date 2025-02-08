/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eaqrabaw <eaqrabaw@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/08 22:24:10 by eaqrabaw          #+#    #+#             */
/*   Updated: 2025/02/08 22:33:00 by eaqrabaw         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	handle_args_in_cmd(t_cmd *cmd, t_token **tokens)
{
	if (*tokens && (*tokens)->type == PIPE)
	{
		cmd->pipe = 1;
		*tokens = (*tokens)->next;
	}
	while (*tokens && ((*tokens)->type == REDIR_IN
			|| (*tokens)->type == REDIR_OUT || (*tokens)->type == APPEND))
	{
		handle_redirection(cmd, *tokens);
		*tokens = (*tokens)->next->next;
	}
	while (*tokens && (*tokens)->type == WORD)
	{
		add_argument(cmd, (*tokens)->value);
		*tokens = (*tokens)->next;
	}
}

t_cmd	*create_new_cmd(t_cmd **current)
{
	t_cmd	*new_comd;

	new_comd = new_cmd();
	if (!new_comd)
		return (NULL);
	if (*current)
		(*current)->next = new_comd;
	*current = new_comd;
	return (new_comd);
}

t_cmd	*parse_tokens(t_token *tokens)
{
	t_cmd	*head;
	t_cmd	*current;

	head = NULL;
	current = NULL;
	while (tokens)
	{
		if (!head)
		{
			head = create_new_cmd(&current);
			if (!head)
				return (NULL);
		}
		else
		{
			if (!create_new_cmd(&current))
			{
				free_cmds_list(head);
				return (NULL);
			}
		}
		handle_args_in_cmd(current, &tokens);
	}
	return (head);
}
