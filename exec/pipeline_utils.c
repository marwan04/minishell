/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipeline_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: malrifai <malrifai@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/05 14:03:18 by malrifai          #+#    #+#             */
/*   Updated: 2025/05/05 14:23:42 by malrifai         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	create_pipes(int pipe_count, int (*pipes)[2])
{
	int	j;
	int	i;

	i = 0;
	while (i < pipe_count)
	{
		if (pipe(pipes[i]) < 0)
		{
			perror("pipe");
			j = 0;
			while (j < i)
			{
				close(pipes[j][0]);
				close(pipes[j][1]);
				j++;
			}
			return (1);
		}
		i++;
	}
	return (0);
}

void	close_all_pipes(int pipe_count, int (*pipes)[2])
{
	int	i;

	i = 0;
	while (i < pipe_count)
	{
		close(pipes[i][0]);
		close(pipes[i][1]);
		i++;
	}
}

int	count_pipes(t_ast *node)
{
	t_ast	*cur;
	int		count;

	count = 1;
	cur = node->right;
	while (cur && cur->type == NODE_PIPE)
	{
		count++;
		cur = cur->right;
	}
	return (count);
}
