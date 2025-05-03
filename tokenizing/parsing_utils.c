/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eaqrabaw <eaqrabaw@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/08 22:24:10 by eaqrabaw          #+#    #+#             */
/*   Updated: 2025/05/04 01:21:32 by eaqrabaw         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_ast	*parse_command(t_token **tokens)
{
	t_ast	*cmd;

	if (*tokens && (*tokens)->type == LPAREN)
		return (parse_group(tokens));
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

t_ast	*parse_pipeline(t_token **tokens)
{
	t_ast	*left;
	t_ast	*right;
	t_ast	*pipe_node;

	left = parse_command(tokens);
	if (!left)
		return (NULL);
	while (*tokens && (*tokens)->type == PIPE)
	{
		*tokens = (*tokens)->next;
		right = parse_command(tokens);
		if (!right)
			return (free_ast(left), NULL);
		pipe_node = new_pipe_node(left, right);
		if (!pipe_node)
			return (free_ast(left), free_ast(right), NULL);
		left = pipe_node;
	}
	return (left);
}

t_ast	*parse_and(t_token **tokens)
{
	t_ast	*left;
	t_ast	*right;
	t_ast	*and_node;

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

t_ast	*parse_expression(t_token **tokens)
{
	t_ast	*left;
	t_ast	*right;
	t_ast	*or_node;

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

t_ast	*parse_group(t_token **tokens)
{
	t_ast	*group_node;
	t_ast	*subtree;

	if (!*tokens || (*tokens)->type != LPAREN)
		return (NULL);
	*tokens = (*tokens)->next;
	subtree = parse_expression(tokens);
	if (!*tokens || (*tokens)->type != RPAREN)
		return (NULL);
	*tokens = (*tokens)->next;
	group_node = malloc(sizeof(t_ast));
	if (!group_node)
		return (NULL);
	group_node->type = NODE_GROUP;
	group_node->left = subtree;
	group_node->right = NULL;
	group_node->args = NULL;
	group_node->file = NULL;
	return (group_node);
}
