/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: malrifai <malrifai@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/08 14:07:09 by malrifai          #+#    #+#             */
/*   Updated: 2025/04/12 16:58:21 by malrifai         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

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

void	add_argument(t_ast *node, char *arg)
{
	int		count;
	int		i;
	char	**new_args;

	count = 0;
	i = 0;
	if (!node || node->type != NODE_CMD)
		return ;
	while (node->args && node->args[count])
		count++;
	new_args = malloc(sizeof(char *) * (count + 2));
	if (!new_args)
		return ;
	while (i < count)
	{
		new_args[i] = node->args[i];
		i++;
	}
	new_args[count] = ft_strdup(arg);
	new_args[count + 1] = NULL;
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
	if (token->type == REDIR_IN)
		redir_node->type = NODE_REDIR_IN;
	else if (token->type == REDIR_OUT)
		redir_node->type = NODE_REDIR_OUT;
	else if (token->type == APPEND)
		redir_node->type = NODE_APPEND;
	else if (token->type == HEREDOC)
		redir_node->type = NODE_HEREDOC;
	redir_node->file = ft_strdup(token->next->value);
	redir_node->left = cmd_node;
	redir_node->right = NULL;
	redir_node->args = NULL;
	return (redir_node);
}
