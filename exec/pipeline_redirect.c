/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipeline_redirect.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: malrifai <malrifai@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/05 14:00:50 by malrifai          #+#    #+#             */
/*   Updated: 2025/05/05 14:20:29 by malrifai         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	handle_pipe_redirects(t_redirect_args *a, int used_in, int used_out)
{
	int	j;
	int	rd;
	int	wr;

	j = 0;
	while (j < a->pipe_count)
	{
		rd = a->pipes[j][0];
		wr = a->pipes[j][1];
		if (j == a->index - 1 && !used_in)
			dup2(rd, STDIN_FILENO);
		if (j == a->index && !used_out)
			dup2(wr, STDOUT_FILENO);
		close(rd);
		close(wr);
		j++;
	}
}

static void	detect_redirect_usage(t_ast *r, int *in, int *out)
{
	*in = 0;
	*out = 0;
	while (r && (r->type == NODE_HEREDOC || r->type == NODE_REDIR_IN
			|| r->type == NODE_REDIR_OUT || r->type == NODE_APPEND))
	{
		if (r->type == NODE_HEREDOC || r->type == NODE_REDIR_IN)
			*in = 1;
		if (r->type == NODE_REDIR_OUT || r->type == NODE_APPEND)
			*out = 1;
		r = r->right;
	}
}

void	exec_setup_redirects(t_redirect_args *a)
{
	int	used_in;
	int	used_out;

	detect_redirect_usage(a->stage, &used_in, &used_out);
	handle_pipe_redirects(a, used_in, used_out);
}

void	handle_redir_child(t_ast *node, int prev_fd, t_minishell *data)
{
	ign_sig();
	if (prev_fd > STDERR_FILENO)
		handle_prev_fd(prev_fd);
	apply_redirections(node);
	if (node->left)
		exit(exec_ast(node->left, STDIN_FILENO, data));
	if (data)
		ft_free(data, data->last_exit_status, "");
	exit(0);
}
