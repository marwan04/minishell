/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eaqrabaw <eaqrabaw@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/08 14:07:09 by malrifai          #+#    #+#             */
/*   Updated: 2025/04/20 09:55:01 by eaqrabaw         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_ast *parse_ast(t_token **tokens)
{
	return (parse_expression(tokens));
}

t_ast	*new_ast_cmd(void)
{
	t_ast	*node;

	node = malloc(sizeof(t_ast));
	if (!node)
		return (NULL);
	node->type = NODE_CMD;
	node->args = NULL;
	node->file = NULL;
	node->left = NULL;
	node->right = NULL;
	return (node);
}

static char	**grow_args(char **old_args, char *new_arg)
{
	int		count;
	int		i;
	char	**new_args;

	i = 0;
	count = 0;
	while (old_args && old_args[count])
		count++;
	new_args = malloc(sizeof(char *) * (count + 2));
	if (!new_args)
		return (NULL);
	while (i < count)
	{
		new_args[i] = old_args[i];
		i++;
	}
	new_args[count] = ft_strdup(new_arg);
	if (!new_args[count])
	{
		free(new_args);
		return (NULL);
	}
	new_args[count + 1] = NULL;
	return (new_args);
}

void	add_argument(t_ast *node, char *arg)
{
	char	**new_args;

	if (!node || node->type != NODE_CMD)
		return ;
	new_args = grow_args(node->args, arg);
	if (!new_args)
		return ;
	free(node->args);
	node->args = new_args;
}

t_ast	*handle_redirection(t_ast *cmd_node, t_token *token)
{
	t_ast	*redir_node;

	if (!cmd_node || !token || !token->next)
		return (NULL);
	redir_node = malloc(sizeof(t_ast));
	if (!redir_node)
		return (NULL);
	redir_node->file = ft_strdup(token->next->value);
	if (!redir_node->file)
	{
		free(redir_node);
		return (NULL);
	}
	redir_node->left = cmd_node;
	redir_node->right = NULL;
	redir_node->args = NULL;
	if (token->type == REDIR_IN)
		redir_node->type = NODE_REDIR_IN;
	else if (token->type == REDIR_OUT)
		redir_node->type = NODE_REDIR_OUT;
	else if (token->type == APPEND)
		redir_node->type = NODE_APPEND;
	else if (token->type == HEREDOC)
		handle_heredoc(redir_node, token);
	return (redir_node);
}

// void	add_argument(t_ast *node, char *arg)
// {
// 	NORM ERROR MORE THAN 25 LINES
// 	int		count;
// 	int		i;
// 	char	**new_args;

// 	count = 0;
// 	i = 0;
// 	if (!node || node->type != NODE_CMD)
// 		return ;
// 	while (node->args && node->args[count])
// 		count++;
// 	new_args = malloc(sizeof(char *) * (count + 2));
// 	if (!new_args)
// 		return ;
// 	while (i < count)
// 	{
// 		new_args[i] = node->args[i];
// 		i++;
// 	}
// 	new_args[count] = ft_strdup(arg);
// 	if (!new_args[count])
// 	{
// 		free(new_args);
// 		return ;
// 	}
// 	new_args[count + 1] = NULL;
// 	free(node->args);
// 	node->args = new_args;
// }