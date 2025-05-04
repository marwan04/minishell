/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   read.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eaqrabaw <eaqrabaw@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/07 06:46:53 by eaqrabaw          #+#    #+#             */
/*   Updated: 2025/05/04 02:03:59 by eaqrabaw         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void ft_read(t_minishell *data)
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
            if (data->ast_root->type == NODE_CMD
                && data->ast_root->args
                && !ft_strcmp(data->ast_root->args[0], "exit"))
            {
                ft_exit(data);
            }
            else
            {
                exec_ast(data->ast_root, -1, data);
            }
            free_tokens(data);
            free_ast(data->ast_root);
            data->ast_root = NULL;
        }
    }
    else
    {
        free(input);  // if you got a SIGINT, still free
    }
}