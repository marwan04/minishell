/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_and_or.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eaqrabaw <eaqrabaw@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/04 07:07:52 by eaqrabaw          #+#    #+#             */
/*   Updated: 2025/05/04 22:40:46 by eaqrabaw         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"	

int	exec_and_or(t_ast *node, int prev_fd, t_minishell *data)
{
    int lhs;
    
    if (!node || !node->left)
        return (0);
        
    lhs = exec_ast(node->left, prev_fd, data);
    
    if ((node->type == NODE_AND && lhs != 0) || 
        (node->type == NODE_OR && lhs == 0) ||
        !node->right)
        return (lhs);
    return (exec_ast(node->right, STDIN_FILENO, data));
}