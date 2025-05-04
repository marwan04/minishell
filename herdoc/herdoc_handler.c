/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   herdoc_handler.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: malrifai <malrifai@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/04 07:03:21 by eaqrabaw          #+#    #+#             */
/*   Updated: 2025/05/04 23:36:57 by malrifai         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	read_heredoc(const char *delim, int write_fd, int expand, t_minishell *data)
{
	char	*line;
	char	*expanded;

	signal(SIGINT, SIG_DFL);
	while (1)
	{
		line = readline("> ");
		if (!line)
		{
			write(STDOUT_FILENO, "\n", 1);
			break ;
		}
		if (strcmp(line, delim) == 0)
		{
			free(line);
			break ;
		}
		if (expand)
		{
			expanded = expand_vars(line, data->env);
			free(line);
			line = expanded;
			if (!line)
				continue ;
		}
		write(write_fd, line, ft_strlen(line));
		write(write_fd, "\n", 1);
		free(line);
	}
	close(write_fd);
}

static void	process_heredoc_node(t_ast *node, t_minishell *data)
{
	int					status;
	pid_t				pid;
	struct sigaction	old_int;
	struct sigaction	new_int;

	if (!node || !node->heredoc_delim)
		return ;
	if (pipe(node->heredoc_pipe) < 0)
	{
		perror("pipe");
		data->last_exit_status = 1;
		return ;
	}
	pid = fork();
	if (pid < 0)
	{
		perror("fork");
		close(node->heredoc_pipe[0]);
		close(node->heredoc_pipe[1]);
		data->last_exit_status = 1;
		return ;
	}
	if (pid == 0)
	{
		sigaction(SIGINT, NULL, &old_int);
		new_int.sa_handler = SIG_DFL;
		sigemptyset(&new_int.sa_mask);
		new_int.sa_flags = 0;
		sigaction(SIGINT, &new_int, NULL);
		close(node->heredoc_pipe[0]);
		read_heredoc(node->heredoc_delim,
			node->heredoc_pipe[1],
			node->heredoc_expand,
			data);
		exit(EXIT_SUCCESS);
	}
	close(node->heredoc_pipe[1]);
	if (waitpid(pid, &status, 0) < 0)
	{
		perror("waitpid");
		close(node->heredoc_pipe[0]);
		data->last_exit_status = 1;
		return ;
	}
	if (WIFSIGNALED(status) && WTERMSIG(status) == SIGINT)
	{
		data->last_exit_status = 130;
	}
}

void	collect_heredocs(t_ast *node, t_minishell *data)
{
	if (!node)
		return ;
	if (node->type == NODE_HEREDOC)
		process_heredoc_node(node, data);   
	if (node->left)
		collect_heredocs(node->left, data);   
	if (node->right)
		collect_heredocs(node->right, data);
}
