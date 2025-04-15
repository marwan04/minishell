/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   read.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: malrifai <malrifai@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/07 06:46:53 by eaqrabaw          #+#    #+#             */
/*   Updated: 2025/04/15 14:14:03 by malrifai         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	ft_exit(t_minishell *data)
{
	if (data->env)
		free_env(data->env);
	if (!data->ast_root->args[1])
		ft_free(data, data->last_exit_status, "exit");
	if (!ft_isnumeric(data->ast_root->args[1]))
	{
		ft_putstr_fd("bash: exit: ", 2);
		ft_putstr_fd(data->ast_root->args[1], 2);
		ft_putendl_fd(": numeric argument required", 2);
		ft_free(data, 2, "exit");
	}
	if (data->ast_root->args[2])
	{
		write(1, "exit\n", 5);
		ft_putendl_fd("bash: exit: too many arguments", 2);
		data->last_exit_status = 1;
		return ;
	}
	data->last_exit_status = ft_atoi(data->ast_root->args[1]);
	ft_free(data, data->last_exit_status, "exit");
}

void	ft_handle_exit(t_minishell *data)
{
	if (data->ast_root && data->ast_root->type == NODE_CMD
		&& data->ast_root->args && !ft_strcmp(data->ast_root->args[0], "exit"))
		ft_exit(data);
}

void	ft_process_input(t_minishell *data, char *input)
{
	t_token	*head;

	if (*input)
	{
		add_history(input);
		free_tokens(data);
		free_ast(data->ast_root);
		head = tokenizer(input);
		data->tokens = head;
		if (head)
		{
			expand_tokens(head, data->last_exit_status, data->env);
			data->ast_root = parse_ast(&head);
		}
	}
}

void	ft_read(t_minishell *data)
{
	char	*input;

	signals_handler();
	input = readline("minishell> ");
	if (!input)
		ft_free(data, 1, "exit");
	ft_process_input(data, input);
	if (data->ast_root)
	{
		ft_handle_exit(data);
		exec_ast(data->ast_root, -1, data);
	}
	free(input);
}
