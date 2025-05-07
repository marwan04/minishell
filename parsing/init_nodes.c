/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_nodes.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eaqrabaw <eaqrabaw@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/30 15:49:26 by malrifai          #+#    #+#             */
/*   Updated: 2025/05/04 01:05:24 by eaqrabaw         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*get_del(char *raw)
{
	if ((raw[0] == '\'' && raw[ft_strlen(raw) - 1] == '\'')
		|| (raw[0] == '"' && raw[ft_strlen(raw) - 1] == '"'))
		return (ft_substr(raw, 1, ft_strlen(raw) - 2));
	else
		return (ft_strdup(raw));
}

t_ast	*new_pipe_node(t_ast *left, t_ast *right)
{
	t_ast	*pipe_node;

	pipe_node = malloc(sizeof(t_ast));
	if (!pipe_node)
		return (NULL);
	pipe_node->type = NODE_PIPE;
	pipe_node->left = left;
	pipe_node->right = right;
	pipe_node->args = NULL;
	pipe_node->file = NULL;
	return (pipe_node);
}

t_ast	*create_redir_node(t_ast *cmd_node, t_token *token)
{
	t_ast	*redir_node;

	redir_node = malloc(sizeof(t_ast));
	if (!redir_node)
		return (NULL);
	redir_node->left = cmd_node;
	redir_node->right = NULL;
	redir_node->args = NULL;
	redir_node->heredoc_expand = 0;
	redir_node->file = get_del(token->next->value);
	return (redir_node);
}
