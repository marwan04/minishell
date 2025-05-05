/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_cmd.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: malrifai <malrifai@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/04 07:26:17 by eaqrabaw          #+#    #+#             */
/*   Updated: 2025/05/05 19:09:15 by malrifai         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static pid_t	exec_fork(int prev_fd)
{
	pid_t	pid;

	pid = fork();
	if (pid < 0)
	{
		perror("fork");
		if (prev_fd > STDERR_FILENO)
			close(prev_fd);
		return (-1);
	}
	return (pid);
}

static int	exec_cmd_execve(t_ast *node, t_minishell *data)
{
	char	*path;
	char	**envp;

	path = ft_get_path(node->args[0], &data->env);
	if (!path)
	{
		ft_putstr_fd(node->args[0], 2);
		ft_putstr_fd(": command not found\n", 2);
		ft_free(data, 127, "");
	}
	envp = envp_to_array(data->env);
	if (!envp)
		ft_free(data, 126, "malloc");
	execve(path, node->args, envp);
	perror("execve");
	free(path);
	ft_free_double_list(envp);
	ft_free(data, 126, "");
	return (126);
}

int	is_parent_builtin(t_ast *node, int prev_fd)
{
	if (node->args[0] && is_builtin(node->args[0])
		&& prev_fd == STDIN_FILENO && node->type == NODE_CMD
		&& !node->right)
		return (1);
	else if (is_builtin(node->args[0]) && !node->right)
		return (1);
	return (0);
}

void	exec_cmd_child(t_ast *node, int prev_fd, t_minishell *data)
{
	def_sig();
	if (prev_fd > STDIN_FILENO)
		handle_prev_fd(prev_fd);
	apply_redirections(node);
	if (!node->args || !node->args[0])
	{
		close(node->heredoc_pipe[0]);
		close(node->heredoc_pipe[1]);
		ft_free(data, 127, "");
	}
	if (is_builtin(node->args[0]))
	{
		data->last_exit_status = 0;
		exec_builtin(node, data, &data->last_exit_status);
		ft_free(data, data->last_exit_status, "");
	}
	exit(exec_cmd_execve(node, data));
}

int	exec_cmd_node(t_ast *node, int prev_fd, t_minishell *data)
{
	int		status;
	pid_t	pid;

	if (!node || !node->args)
		return (0);
	if (is_parent_builtin(node, prev_fd))
		return (exec_builtin(node, data, &status));
	pid = exec_fork(prev_fd);
	if (pid < 0)
		return (data->last_exit_status = 1);
	ign_sig();
	if (pid == 0)
		exec_cmd_child(node, prev_fd, data);
	if (prev_fd > STDERR_FILENO)
		close(prev_fd);
	waitpid(pid, &status, 0);
	init_signals();
	set_exit_status_from_wait(status, data);
	return (data->last_exit_status);
}
