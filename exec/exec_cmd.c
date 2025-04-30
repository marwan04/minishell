/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_cmd.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: malrifai <malrifai@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/30 17:53:11 by malrifai          #+#    #+#             */
/*   Updated: 2025/04/30 17:53:37 by malrifai         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	exec_cmd_child(t_ast *node, int prev_fd, t_minishell *data)
{
	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
	if (prev_fd != -1)
	{
		dup2(prev_fd, STDIN_FILENO);
		close(prev_fd);
	}
	if (is_builtin(node->args[0]))
		execute_builtin_cmds(node, &data->last_exit_status, &data->env);
	else
		ft_execute_command(node, data);
	exit(data->last_exit_status);
}

static void	wait_for_cmd(pid_t pid, int prev_fd, t_minishell *data)
{
	int	status;

	if (prev_fd != -1)
		close(prev_fd);
	waitpid(pid, &status, 0);
	init_signals();
	data->last_exit_status = WEXITSTATUS(status);
}

int	handle_cmd_node(t_ast *node, int prev_fd, t_minishell *data)
{
	pid_t	pid;
	int		in_pipe;

	if (!node || !node->args)
	{
		data->last_exit_status = 127;
		return (127);
	}
	in_pipe = (prev_fd != -1);
	if (is_builtin(node->args[0]) && !in_pipe)
	{
		execute_builtin_cmds(node, &data->last_exit_status, &data->env);
		return (0);
	}
	signal(SIGINT, SIG_IGN);
	signal(SIGQUIT, SIG_IGN);
	pid = fork();
	if (pid == 0)
		exec_cmd_child(node, prev_fd, data);
	wait_for_cmd(pid, prev_fd, data);
	return (data->last_exit_status);
}
