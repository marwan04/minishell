/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   herdoc_handler.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eaqrabaw <eaqrabaw@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/20 06:38:24 by eaqrabaw          #+#    #+#             */
/*   Updated: 2025/04/20 08:04:27 by eaqrabaw         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int     process_heredoc(t_minishell *data, t_ast *node)
{
    
}

int     handle_heredoc(t_minishell *data)
{
    t_ast *node = data->ast_root;
    
    while (node)
    {
        if (node->type == NODE_HEREDOC)
        {
            if (process_heredoc(data, node) == HEREDOC_INTERRUPTED)
                return (HEREDOC_INTERRUPTED);
        }
        if (node->left)
            handle_heredoc(data); 
        node = node->right;
    }
    return (0);
}
