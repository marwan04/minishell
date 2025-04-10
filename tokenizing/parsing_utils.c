/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: malrifai <malrifai@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/08 22:24:10 by eaqrabaw          #+#    #+#             */
/*   Updated: 2025/04/10 19:03:20 by malrifai         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

t_ast *parse_command(t_token **tokens)
{
	t_ast *cmd = new_ast_cmd();

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

	// ⬇️ After args, check for any more redirections
	while (*tokens && ((*tokens)->type == REDIR_IN
		|| (*tokens)->type == REDIR_OUT || (*tokens)->type == APPEND
		|| (*tokens)->type == HEREDOC))
	{
		cmd = handle_redirection(cmd, *tokens);
		*tokens = (*tokens)->next->next;
	}
	return cmd;
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

t_ast *parse_pipeline(t_token **tokens)
{
	t_ast *left = parse_command(tokens);

	while (*tokens && (*tokens)->type == PIPE)
	{
		*tokens = (*tokens)->next; // skip the PIPE token
		t_ast *right = parse_command(tokens);

		t_ast *pipe_node = malloc(sizeof(t_ast));
		if (!pipe_node)
			return NULL;

		pipe_node->type = NODE_PIPE;
		pipe_node->left = left;
		pipe_node->right = right;
		pipe_node->args = NULL;
		pipe_node->file = NULL;

		left = pipe_node; // chain pipe to the left
	}

	return left;
}

t_ast *parse_ast(t_token **tokens)
{
	return parse_pipeline(tokens); // for now, pipeline is the entry point
}
