/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eaqrabaw <eaqrabaw@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/08 14:07:09 by malrifai          #+#    #+#             */
/*   Updated: 2025/05/04 01:20:32 by eaqrabaw         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_ast	*parse_ast(t_token **tokens)
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

t_ast	*handle_redirection(t_ast *cmd_node, t_token *token)
{
	t_ast	*redir_node;

	if (!cmd_node || !token || !token->next || !token->next->value)
		return (NULL);
	redir_node = create_redir_node(cmd_node, token);
	if (!redir_node)
		return (NULL);
	if (token->type == HEREDOC)
	{
		redir_node->heredoc_delim = redir_node->file;
		redir_node->heredoc_expand = !token->next->herdoc_quote;
		redir_node->type = NODE_HEREDOC;
	}
	else if (token->type == REDIR_IN)
		redir_node->type = NODE_REDIR_IN;
	else if (token->type == REDIR_OUT)
		redir_node->type = NODE_REDIR_OUT;
	else if (token->type == APPEND)
		redir_node->type = NODE_APPEND;
	return (redir_node);
}

static void	free_new_args(char **args, int until)
{
	while (--until >= 0)
		free(args[until]);
	free(args);
}

void	add_argument(t_ast *node, char *arg)
{
	int		count;
	int		i;
	char	**new_args;

	if (!node || node->type != NODE_CMD)
		return ;
	count = 0;
	while (node->args && node->args[count])
		count++;
	new_args = malloc(sizeof(char *) * (count + 2));
	if (!new_args)
		return ;
	i = 0;
	while (i < count)
	{
		new_args[i] = node->args[i];
		i++;
	}
	new_args[count] = ft_strdup(arg);
	if (!new_args[count])
		return (free_new_args(new_args, count));
	new_args[count + 1] = NULL;
	free(node->args);
	node->args = new_args;
}
