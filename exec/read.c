/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   read.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: malrifai <malrifai@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/07 06:46:53 by eaqrabaw          #+#    #+#             */
/*   Updated: 2025/04/30 15:24:31 by malrifai         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_handle_exit(t_minishell *data)
{
	if (data->ast_root && data->ast_root->type == NODE_CMD
		&& data->ast_root->args && !ft_strcmp(data->ast_root->args[0], "exit"))
		ft_exit(data);
}
void free_tokens_from_list(t_token *token)
{
	t_token *next;

	while (token)
	{
		next = token->next;
		if (token->value)
			free(token->value);
		free(token);
		token = next;
	}
}

void	ft_process_input(t_minishell *data, char *input)
{
	t_token *raw_tokens;

	if (*input)
	{
		add_history(input);
		if (syntax_error(input))
		{
			write(2, "minishell: syntax error\n", 25);
			data->last_exit_status = 2;
			free(input);
			free_tokens(data);
			free_ast(data->ast_root);
			return ;
		}
		free_tokens(data);
		free_ast(data->ast_root);
		raw_tokens = tokenizer(input);
		data->tokens = raw_tokens;
		if (validate_token_sequence(data->tokens))
		{
			ft_putendl_fd("minishell: syntax error near unexpected token", 2);
			data->last_exit_status = 2;
			free_tokens_from_list(raw_tokens);
			free_ast(data->ast_root);
			free(input);
			data->tokens = NULL;
			data->ast_root = NULL;
			return ;
		}
		if (data->tokens)
		{
			expand_tokens(data->tokens, data->last_exit_status, data->env);
			expand_wildcards(data->tokens);
			normalize_tokens(&data->tokens);
			normalize_tokens_with_heredoc(&data->tokens);
			data->ast_root = parse_ast(&data->tokens);
			if (!data->ast_root)
			{
				free_tokens_from_list(raw_tokens);
				free(input);
				data->tokens = NULL;
				return ;
			}
			collect_heredocs(data->ast_root, data);
		}
		free_tokens_from_list(raw_tokens);
	}
	free(input);
}

void	ft_read(t_minishell *data)
{
	char *input;

	check_signal(data);
	data->execution_aborted = 0;
	input = readline("minishell> ");
	if (!input && !g_sig_int)
		ft_free(data, 1, "exit\n");

	if (!g_sig_int)
	{
		ft_process_input(data, input);
		if (data->ast_root && !data->execution_aborted)
		{
			ft_handle_exit(data);
			exec_ast(data->ast_root, -1, data);
		}
		free_tokens(data);
		free_ast(data->ast_root);
		data->ast_root = NULL;
		data->tokens = NULL;
	}
}
