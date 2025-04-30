/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   read.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eaqrabaw <eaqrabaw@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/07 06:46:53 by eaqrabaw          #+#    #+#             */
/*   Updated: 2025/04/30 04:42:10 by eaqrabaw         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_handle_exit(t_minishell *data)
{
	if (data->ast_root && data->ast_root->type == NODE_CMD
		&& data->ast_root->args && !ft_strcmp(data->ast_root->args[0], "exit"))
		ft_exit(data);
}

void	ft_process_input(t_minishell *data, char *input)
{
	t_token *head;

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
			return;
		}

		free_tokens(data);
		free_ast(data->ast_root);

		head = tokenizer(input);
		data->tokens = head;

		if (validate_token_sequence(data->tokens))
		{
			ft_putendl_fd("minishell: syntax error near unexpected token", 2);
			data->last_exit_status = 2;
			free_tokens(data);
			free_ast(data->ast_root);
			free(input);
			return;
		}

		if (head)
		{
			expand_tokens(head, data->last_exit_status, data->env);
			expand_wildcards(data->tokens);
			normalize_tokens(&data->tokens);
			normalize_tokens_with_heredoc(&data->tokens);
			data->ast_root = parse_ast(&data->tokens);
			if (!data->ast_root)
			{
				// ✅ Free remaining tokens and input if AST fails
				free_tokens(data);
				free(input);
				return;
			}
			generate_ast_diagram(data->ast_root);
			collect_heredocs(data->ast_root, data);
		}
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
	}
}
