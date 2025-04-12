/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: malrifai <malrifai@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/08 22:24:10 by eaqrabaw          #+#    #+#             */
/*   Updated: 2025/04/12 17:04:35 by malrifai         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

t_ast	*parse_command(t_token **tokens)
{
	t_ast	*cmd;

	cmd = new_ast_cmd();
	while (*tokens && ((*tokens)->type == REDIR_IN
			|| (*tokens)->type == REDIR_OUT || (*tokens)->type == APPEND
			|| (*tokens)->type == HEREDOC))
	{
		cmd = handle_redirection(cmd, *tokens);
		*tokens = (*tokens)->next->next;
	}
	while (*tokens && (*tokens)->type == WORD)
	{
		add_argument(cmd, (*tokens)->value);
		*tokens = (*tokens)->next;
	}
	while (*tokens && ((*tokens)->type == REDIR_IN
			|| (*tokens)->type == REDIR_OUT || (*tokens)->type == APPEND
			|| (*tokens)->type == HEREDOC))
	{
		cmd = handle_redirection(cmd, *tokens);
		*tokens = (*tokens)->next->next;
	}
	return (cmd);
}

// t_cmd	*create_new_cmd(t_cmd **current)
// {
// 	t_cmd	*new_comd;

// 	new_comd = new_cmd();
// 	if (!new_comd)
// 		return (NULL);
// 	if (*current)
// 		(*current)->next = new_comd;
// 	*current = new_comd;
// 	return (new_comd);
// }

t_ast	*parse_ast(t_token **tokens)
{
	t_ast	*left;
	t_ast	*right;
	t_ast	*pipe_node;

	left = parse_command(tokens);
	while (*tokens && (*tokens)->type == PIPE)
	{
		*tokens = (*tokens)->next;
		right = parse_command(tokens);
		pipe_node = malloc(sizeof(t_ast));
		if (!pipe_node)
			return (NULL);
		pipe_node->type = NODE_PIPE;
		pipe_node->left = left;
		pipe_node->right = right;
		pipe_node->args = NULL;
		pipe_node->file = NULL;
		left = pipe_node;
	}
	return (left);
}
