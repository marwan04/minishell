/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: malrifai <malrifai@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/08 22:24:10 by eaqrabaw          #+#    #+#             */
/*   Updated: 2025/04/16 15:40:08 by malrifai         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

t_ast	*parse_command(t_token **tokens)
{
	t_ast	*cmd;

	if (*tokens && (*tokens)->type == LPAREN)
		return parse_group(tokens);
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

t_ast *parse_pipeline(t_token **tokens)
{
	t_ast *left;
	t_ast *right;
	t_ast *pipe_node;
	
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

t_ast *parse_and(t_token **tokens)
{
	t_ast *left;
	t_ast *right;
	t_ast *and_node;
	
	left = parse_pipeline(tokens);
	while (*tokens && (*tokens)->type == AND)
	{
		*tokens = (*tokens)->next;
		right = parse_pipeline(tokens);
		and_node = malloc(sizeof(t_ast));
		if (!and_node)
			return (NULL);
		and_node->type = NODE_AND;
		and_node->left = left;
		and_node->right = right;
		and_node->args = NULL;
		and_node->file = NULL;
		left = and_node;
	}
	return (left);
}

t_ast *parse_expression(t_token **tokens)
{
	t_ast *left;
	t_ast *right;
	t_ast *or_node;
	
	left = parse_and(tokens);
	while (*tokens && (*tokens)->type == OR)
	{
		*tokens = (*tokens)->next;
		right = parse_and(tokens);

		or_node = malloc(sizeof(t_ast));
		if (!or_node)
			return (NULL);

		or_node->type = NODE_OR;
		or_node->left = left;
		or_node->right = right;
		or_node->args = NULL;
		or_node->file = NULL;

		left = or_node;
	}
	return (left);
}

t_ast *parse_group(t_token **tokens)
{
	t_ast *group_node;
	
	if (!*tokens || (*tokens)->type != LPAREN)
		return (NULL);
	*tokens = (*tokens)->next;
	t_ast *subtree = parse_expression(tokens); 
	if (!*tokens || (*tokens)->type != RPAREN)
		return (NULL);
	*tokens = (*tokens)->next;
	group_node = malloc(sizeof(t_ast));
	group_node->type = NODE_GROUP;
	group_node->left = subtree;
	group_node->right = NULL;
	group_node->args = NULL;
	group_node->file = NULL;
	return (group_node);
}

t_ast *parse_ast(t_token **tokens)
{
	return parse_expression(tokens);
}
