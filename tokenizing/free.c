/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eaqrabaw <eaqrabaw@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/08 18:29:42 by malrifai          #+#    #+#             */
/*   Updated: 2025/04/30 04:51:08 by eaqrabaw         ###   ########.fr       */
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

// void	free_cmd(t_cmd *cmd)
// {
// 	int	i;

// 	if (cmd->input)
// 		free(cmd->input);
// 	if (cmd->output)
// 		free(cmd->output);
// 	if (cmd->args)
// 	{
// 		i = 0;
// 		while (cmd->args[i] != NULL)
// 		{
// 			if (cmd->args[i])
// 				free(cmd->args[i]);
// 			i++;
// 		}
// 		free(cmd->args);
// 	}
// 	free(cmd);
// }

// void	free_cmds(t_minishell *data)
// {
// 	t_cmd	*tmp;
// 	t_cmd	*tmp2;

// 	if (!data || !data->cmds)
// 		return ;
// 	tmp = data->cmds;
// 	while (tmp != NULL)
// 	{
// 		tmp2 = tmp->next;
// 		free_cmd(tmp);
// 		tmp = tmp2;
// 	}
// 	data->cmds = NULL;
// }

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

void 	ft_free_after_cmd(t_minishell *data)
{
	if (data->tokens)
		free_tokens(data);
	if (data->ast_root)
		free_ast(data->ast_root);
}