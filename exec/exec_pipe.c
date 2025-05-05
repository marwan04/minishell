/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_pipe.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: malrifai <malrifai@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/04 12:00:00 by student           #+#    #+#             */
/*   Updated: 2025/05/05 14:03:59 by malrifai         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	wait_for_all(pid_t *pids, int n_stages)
{
	int	i;
	int	status;
	int	last_status;

	i = 0;
	last_status = 0;
	while (i < n_stages)
	{
		if (waitpid(pids[i], &status, 0) < 0)
			perror("waitpid");
		else
			last_status = WEXITSTATUS(status);
		i++;
	}
	return (last_status);
}

static int	handle_pipeline_allocation(int pipe_count, int n_stages,
			int (**pipes)[2], pid_t **pids)
{
	*pipes = malloc(pipe_count * sizeof(**pipes));
	*pids = malloc(n_stages * sizeof(**pids));
	if (!*pipes || !*pids)
	{
		perror("malloc");
		free(*pipes);
		free(*pids);
		return (1);
	}
	return (0);
}

int	exec_pipeline_helper(t_exec_pipeline_args *a,
						int pipe_count, int (**pipes)[2], pid_t **pids)
{
	t_fork_args	f;

	if (create_pipes(pipe_count, *pipes))
	{
		free(*pipes);
		free(*pids);
		return (1);
	}
	f.stages = a->stages;
	f.n_stages = a->n_stages;
	f.pipe_count = pipe_count;
	f.prev_fd = a->prev_fd;
	f.pipes = *pipes;
	f.pids = *pids;
	f.data = a->data;
	if (fork_and_exec(&f))
	{
		free(*pipes);
		free(*pids);
		return (1);
	}
	return (0);
}

int	exec_pipeline_main(t_exec_pipeline_args *a, int *last_status)
{
	int			(*pipes)[2];
	pid_t		*pids;
	int			pipe_count;

	pipes = NULL;
	pids = NULL;
	pipe_count = a->n_stages - 1;
	if (handle_pipeline_allocation(pipe_count, a->n_stages, &pipes, &pids))
		return (1);
	if (exec_pipeline_helper(a, pipe_count, &pipes, &pids))
		return (1);
	if (a->prev_fd > STDERR_FILENO)
		close(a->prev_fd);
	close_all_pipes(pipe_count, pipes);
	close_heredoc_pipes_in_stages(a->stages, a->n_stages);
	*last_status = wait_for_all(pids, a->n_stages);
	init_signals();
	free(pipes);
	free(pids);
	return (0);
}

int	exec_pipeline(t_ast **stages, int n_stages, int prev_fd, t_minishell *data)
{
	t_exec_pipeline_args	args;
	int						last_status;

	args.stages = stages;
	args.n_stages = n_stages;
	args.prev_fd = prev_fd;
	args.data = data;
	last_status = 0;
	if (n_stages <= 1)
		return (exec_cmd_node(stages[0], prev_fd, data));
	if (exec_pipeline_main(&args, &last_status))
		return (1);
	return (last_status);
}
