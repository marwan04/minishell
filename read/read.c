/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   read.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: malrifai <malrifai@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/07 06:46:53 by eaqrabaw          #+#    #+#             */
/*   Updated: 2025/05/05 18:44:21 by malrifai         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	free_tokens_from_list(t_token *token)
{
	t_token	*next;

	while (token)
	{
		next = token->next;
		if (token->value)
			free(token->value);
		free(token);
		token = next;
	}
}

void	ft_read(t_minishell *data)
{
	char	*input;

	check_signal(data);
	data->execution_aborted = 0;
	input = readline("minishell> ");
	if (!input && !g_sig_int)
		ft_free(data, 1, "exit\n");
	if (!g_sig_int)
	{
		ft_process_input(data, input);
		if (data->ast_root)
		{
			if (data->ast_root->type == NODE_CMD
				&& data->ast_root->args
				&& !ft_strcmp(data->ast_root->args[0], "exit"))
				ft_exit(data);
			else
				exec_ast(data->ast_root, -1, data);
			free_tokens(data);
			free_ast(data->ast_root);
			data->ast_root = NULL;
		}
	}
	else
		free(input);
}
