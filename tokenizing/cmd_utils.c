/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eaqrabaw <eaqrabaw@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/08 14:07:09 by malrifai          #+#    #+#             */
/*   Updated: 2025/04/30 04:43:03 by eaqrabaw         ###   ########.fr       */
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

char *get_del(char *raw)
{
	if ((raw[0] == '\'' && raw[ft_strlen(raw) - 1] == '\'')
		|| (raw[0] == '"' && raw[ft_strlen(raw) - 1] == '"'))
		return ft_substr(raw, 1, ft_strlen(raw) - 2);
	else
		return ft_strdup(raw);
}

t_ast *handle_redirection(t_ast *cmd_node, t_token *token)
{
    t_ast *redir_node;
    char *raw;

    if (!cmd_node || !token || !token->next || !token->next->value)
        return NULL;

    redir_node = malloc(sizeof(t_ast));
    if (!redir_node)
        return NULL;

    raw = token->next->value;
    redir_node->left = cmd_node;
    redir_node->right = NULL;
    redir_node->args = NULL;
    redir_node->heredoc_expand = 0;
    redir_node->file = get_del(raw);

    if (token->type == HEREDOC)
    {
        redir_node->heredoc_expand = !token->next->herdoc_quote;
        redir_node->type = NODE_HEREDOC;
    }
    else if (token->type == REDIR_IN)
        redir_node->type = NODE_REDIR_IN;
    else if (token->type == REDIR_OUT)
        redir_node->type = NODE_REDIR_OUT;
    else if (token->type == APPEND)
        redir_node->type = NODE_APPEND;

    return redir_node;
}


void add_argument(t_ast *node, char *arg)
{
    int count = 0;
    int i = 0;
    char **new_args;

    if (!node || node->type != NODE_CMD)
        return;

    while (node->args && node->args[count])
        count++;

    new_args = malloc(sizeof(char *) * (count + 2));
    if (!new_args)
        return;

    while (i < count)
    {
        new_args[i] = node->args[i];
        i++;
    }

    new_args[count] = ft_strdup(arg);
    if (!new_args[count])
    {
        while (i-- > 0)
            free(new_args[i]);
        free(new_args);
        return;
    }

    new_args[count + 1] = NULL;
    free(node->args);
    node->args = new_args;
}
