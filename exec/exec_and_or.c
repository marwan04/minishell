/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_and_or.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: malrifai <malrifai@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/04 07:07:52 by eaqrabaw          #+#    #+#             */
/*   Updated: 2025/05/05 00:33:26 by malrifai         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"	

int	exec_and_or(t_ast *node, int prev_fd, t_minishell *data)
{
	int	lhs;

	if (!node || !node->left)
		return (0);
	lhs = exec_ast(node->left, prev_fd, data);
	if ((node->type == NODE_AND && lhs != 0)
		|| (node->type == NODE_OR && lhs == 0)
		|| !node->right)
		return (lhs);
	return (exec_ast(node->right, STDIN_FILENO, data));
}
