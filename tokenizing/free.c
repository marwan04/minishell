/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: malrifai <malrifai@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/08 18:29:42 by malrifai          #+#    #+#             */
/*   Updated: 2025/04/30 14:25:34 by malrifai         ###   ########.fr       */
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
	int	i;

	if (!node)
		return ;
	free_ast(node->left);
	free_ast(node->right);
	if (node->type == NODE_CMD && node->args)
	{
		i = 0;
		while (node->args[i])
		{
			free(node->args[i]);
			i++;
		}
		free(node->args);
	}
	if (node->file)
		free(node->file);
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
