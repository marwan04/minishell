/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: malrifai <malrifai@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/04 07:16:09 by eaqrabaw          #+#    #+#             */
/*   Updated: 2025/05/05 14:25:02 by malrifai         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	count_pipe_leaves(t_ast *node)
{
	int	result;

	if (!node)
		return (0);
	if (node->type == NODE_PIPE)
	{
		result = count_pipe_leaves(node->left);
		result += count_pipe_leaves(node->right);
		return (result);
	}
	return (1);
}

static void	flatten_pipeline_helper(t_ast *node, t_ast **stages, int *idx)
{
	if (!node)
		return ;
	if (node->type == NODE_PIPE)
	{
		flatten_pipeline_helper(node->left, stages, idx);
		flatten_pipeline_helper(node->right, stages, idx);
	}
	else
	{
		stages[(*idx)++] = node;
	}
}

t_ast	**collect_pipeline_stages(t_ast *pipe_root, int *out_n)
{
	int		n;
	t_ast	**stages;
	int		idx;

	n = count_pipe_leaves(pipe_root);
	stages = malloc(sizeof * stages * n);
	idx = 0;
	if (!stages)
	{
		perror("malloc");
		*out_n = 0;
		return (NULL);
	}
	flatten_pipeline_helper(pipe_root, stages, &idx);
	*out_n = n;
	return (stages);
}

static int	exec_pipeline_with_all(t_ast *node,
	int count, int prev_fd, t_minishell *data)
{
	t_ast	**stages;
	int		result;
	int		r;

	stages = malloc(sizeof(t_ast *) * (count + 1));
	if (!stages)
	{
		perror("malloc");
		return (1);
	}
	r = count + 1;
	flatten_pipeline_helper(node, stages, &r);
	result = exec_pipeline(stages, count + 1, prev_fd, data);
	free(stages);
	return (result);
}

int	handle_pipe_node(t_ast *node, int prev_fd, t_minishell *data)
{
	int	pipe_count;

	if (!node || !node->left || !node->right)
		return (1);
	pipe_count = count_pipes(node);
	return (exec_pipeline_with_all(node, pipe_count, prev_fd, data));
}
