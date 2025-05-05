/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   herdoc_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: malrifai <malrifai@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/04 07:25:07 by eaqrabaw          #+#    #+#             */
/*   Updated: 2025/05/05 17:45:40 by malrifai         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	close_heredoc_in_node(t_ast *node)
{
	if (!node)
		return ;
	if (node->type == NODE_HEREDOC
		&& node->heredoc_pipe[0] > STDERR_FILENO)
	{
		close(node->heredoc_pipe[0]);
		node->heredoc_pipe[0] = -1;
	}
	close_heredoc_in_node(node->left);
	close_heredoc_in_node(node->right);
}

void	close_heredoc_pipes_in_stages(t_ast **stages, int n_stages)
{
	int	i;

	i = 0;
	while (i < n_stages)
	{
		close_heredoc_in_node(stages[i]);
		i++;
	}
}

static void	setup_sigint_default(struct sigaction *old_int)
{
	struct sigaction	new_int;

	sigaction(SIGINT, NULL, old_int);
	new_int.sa_handler = SIG_DFL;
	sigemptyset(&new_int.sa_mask);
	new_int.sa_flags = 0;
	sigaction(SIGINT, &new_int, NULL);
}

void	heredoc_child(t_ast *node, t_minishell *data)
{
	struct sigaction	old_int;

	setup_sigint_default(&old_int);
	close(node->heredoc_pipe[0]);
	read_heredoc(node->heredoc_delim,
		node->heredoc_pipe[1],
		node->heredoc_expand,
		data);
	exit(EXIT_SUCCESS);
}

void	close_heredocs(t_ast *node)
{
	if (node->heredoc_pipe[0] > STDERR_FILENO)
	{
		close(node->heredoc_pipe[0]);
		node->heredoc_pipe[0] = -1;
	}
	if (node->heredoc_pipe[1] > STDERR_FILENO)
	{
		close(node->heredoc_pipe[1]);
		node->heredoc_pipe[1] = -1;
	}
}
