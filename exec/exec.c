/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eaqrabaw <eaqrabaw@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/17 18:58:37 by malrifai          #+#    #+#             */
/*   Updated: 2025/05/05 02:12:47 by eaqrabaw         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	exec_group_node(t_ast *node, int prev_fd, t_minishell *data)
{
	pid_t	pid;
	int		status;

	signal(SIGINT, SIG_IGN);
	signal(SIGQUIT, SIG_IGN);
	pid = fork();
    status = 0;
	if (pid < 0)
        return (handle_fork_error(prev_fd, data));
	if (pid == 0)
	{
		signal(SIGINT, SIG_DFL);
		signal(SIGQUIT, SIG_DFL);
		if (prev_fd > STDERR_FILENO && prev_fd != -1)
            handle_prev_fd(prev_fd);
		exit(exec_ast(node->left, STDIN_FILENO, data));
	}
	if (prev_fd > STDERR_FILENO)
		close(prev_fd);
	waitpid(pid, &status, 0);
	init_signals();
	if (WIFSIGNALED(status))
		data->last_exit_status = 128 + WTERMSIG(status);
	else
		data->last_exit_status = WEXITSTATUS(status);
	return (data->last_exit_status);
}

static int	exec_redir_node(t_ast *node, int prev_fd, t_minishell *data)
{
	pid_t	pid;
	int		status;

    status = 0;

	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
	pid = fork();
	if (pid < 0)
        return (handle_fork_error(prev_fd, data));
	if (pid == 0)
	{
		signal(SIGINT, SIG_IGN);
		signal(SIGQUIT, SIG_IGN);
		if (prev_fd > STDERR_FILENO)
            handle_prev_fd(prev_fd);
		apply_redirections(node);
		if (node->left)
			exit(exec_ast(node->left, STDIN_FILENO, data));
		exit(0);
	}
	if (prev_fd > STDERR_FILENO)
		close(prev_fd);
	waitpid(pid, &status, 0);
	init_signals();
	if (WIFSIGNALED(status))
		data->last_exit_status = 128 + WTERMSIG(status);
	else
		data->last_exit_status = WEXITSTATUS(status);
	return (data->last_exit_status);
}

static int	exec_pipeline_node(t_ast *node, int prev_fd, t_minishell *data)
{
	int		n_stages;
	t_ast	**stages;
	int		status;

	stages = collect_pipeline_stages(node, &n_stages);
	if (!stages)
		return (1);
	status = exec_pipeline(stages, n_stages, prev_fd, data);
	free(stages);
	return (status);
}

static int	exec_ast_helper(t_ast *node, int prev_fd, t_minishell *data)
{
	if (node->type == NODE_PIPE)
		return (exec_pipeline_node(node, prev_fd, data));
	else if (node->type == NODE_AND || node->type == NODE_OR)
		return (exec_and_or(node, prev_fd, data));
	else if (node->type == NODE_GROUP)
		return (exec_group_node(node, prev_fd, data));
	else if (node->type == NODE_CMD)
		return (exec_cmd_node(node, prev_fd, data));
	else if (node->type == NODE_HEREDOC || node->type == NODE_REDIR_IN
		|| node->type == NODE_REDIR_OUT || node->type == NODE_APPEND)
		return (exec_redir_node(node, prev_fd, data));
	else
	{
        ft_putstr_fd("minishell: Unknown AST node type: ", 2);
        ft_putnbr_fd(node->type, 2);
        ft_putchar_fd('\n', 2);
		return (1);
	}
}

int	exec_ast(t_ast *node, int prev_fd, t_minishell *data)
{
	if (!node)
		return (0);
	collect_heredocs(node, data);
	if (data->last_exit_status == 130)
	{
		data->pipes_count = 0;
		if (prev_fd > STDERR_FILENO || prev_fd != -1)
			close(prev_fd);
		return (131);
	}
	data->last_exit_status = exec_ast_helper(node, prev_fd, data);
	if (prev_fd > STDERR_FILENO)
		close(prev_fd);
	return (data->last_exit_status);
}
