/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   herdoc_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eaqrabaw <eaqrabaw@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/04 07:25:07 by eaqrabaw          #+#    #+#             */
/*   Updated: 2025/05/04 07:34:14 by eaqrabaw         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void    close_heredoc_in_node(t_ast *node)
{
    if (!node) return;
    if (node->type == NODE_HEREDOC
     && node->heredoc_pipe[0] > STDERR_FILENO)
    {
        close(node->heredoc_pipe[0]);
        node->heredoc_pipe[0] = -1;
    }
    close_heredoc_in_node(node->left);
    close_heredoc_in_node(node->right);
}

void close_heredoc_pipes_in_stages(t_ast **stages, int n_stages)
{
    for (int i = 0; i < n_stages; i++)
        close_heredoc_in_node(stages[i]);
}