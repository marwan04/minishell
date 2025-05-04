/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   read_process.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eaqrabaw <eaqrabaw@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/04 00:17:16 by eaqrabaw          #+#    #+#             */
/*   Updated: 2025/05/05 01:51:12 by eaqrabaw         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	handle_raw_syntax(t_minishell *data, char *input)
{
	if (syntax_error(input))
	{
		write(2, "minishell: syntax error\n", 25);
		data->last_exit_status = 2;
		free(input);
		free_tokens(data);
		free_ast(data->ast_root);
		return (1);
	}
	return (0);
}

static int	handle_token_sequence(t_minishell *data,
	t_token *raw_tokens, char *input)
{
	if (validate_token_sequence(raw_tokens))
	{
		ft_putendl_fd("minishell: syntax error near unexpected token", 2);
		data->last_exit_status = 2;
		free_tokens_from_list(raw_tokens);
		free_ast(data->ast_root);
		free(input);
		data->tokens = NULL;
		data->ast_root = NULL;
		return (1);
	}
	return (0);
}

static int	handle_parsing(t_minishell *data,
	t_token *raw_tokens, char *input)
{
	data->ast_root = parse_ast(&raw_tokens);
	if (!data->ast_root)
	{
		free_tokens_from_list(raw_tokens);
		free(input);
		data->tokens = NULL;
		return (1);
	}
	return (0);
}

int	ft_pipes_count(t_token *tokens)
{
	t_token	*token;
	int		count;

	count = 0;
	token = tokens;
	while (token)
	{
		if (token->type == PIPE)
			count++;
		token = token->next;
	}
	return (count);
}

void	ft_process_input(t_minishell *data, char *input)
{
	t_token	*raw_tokens;

	if (!*input)
		return (free(input));
	add_history(input);
	if (handle_raw_syntax(data, input))
		return ;
	free_tokens(data);
	free_ast(data->ast_root);
	raw_tokens = tokenizer(input);
	data->tokens = raw_tokens;
	data->pipes_count = ft_pipes_count(raw_tokens);
	if (handle_token_sequence(data, raw_tokens, input))
		return ;
	if (data->tokens)
	{
		expand_tokens(data->tokens, data->last_exit_status, data->env);
		expand_wildcards(data->tokens);
		normalize_tokens(&data->tokens);
		if (handle_parsing(data, raw_tokens, input))
			return ;
	}
	g_sig_int = 0;
	free(input);
}
