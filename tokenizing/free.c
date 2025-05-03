/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eaqrabaw <eaqrabaw@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/08 18:29:42 by malrifai          #+#    #+#             */
/*   Updated: 2025/05/04 02:06:24 by eaqrabaw         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	free_tokens(t_minishell *data)
{
	t_token	*tmp;
	t_token	*tmp2;

	if (!data || !data->tokens)
		return ;
	tmp = data->tokens;
	while (tmp != NULL)
	{
		if (tmp->value)
			free(tmp->value);
		tmp2 = tmp->next;
		free(tmp);
		tmp = tmp2;
	}
	data->tokens = NULL;
}

void	free_ast(t_ast *node)
{
	if (!node)
		return;

	// First free children
	if (node->left)
	{
		free_ast(node->left);
		node->left = NULL;
	}
	if (node->right)
	{
		free_ast(node->right);
		node->right = NULL;
	}

	// Then free arguments if it's a command node
	if (node->type == NODE_CMD && node->args)
	{
		int i = 0;
		while (node->args[i])
		{
			free(node->args[i]);
			node->args[i] = NULL;
			i++;
		}
		free(node->args);
		node->args = NULL;
	}

	// Finally free file if it exists
	if (node->file)
	{
		free(node->file);
		node->file = NULL;
	}

	// Free the node itself
	free(node);
}

void	ft_free(t_minishell *data, int flag, char *msg)
{
	if (data->tokens)
		free_tokens(data);
	if (data->ast_root)
		free_ast(data->ast_root);
	if (data->env)
		free_env(data->env);
	printf("%s", msg);
	exit(flag);
}

void	ft_free_after_cmd(t_minishell *data)
{
	if (data->tokens)
		free_tokens(data);
	if (data->ast_root)
		free_ast(data->ast_root);
}
