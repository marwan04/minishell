/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error_utilites.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: malrifai <malrifai@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/24 09:22:11 by eaqrabaw          #+#    #+#             */
/*   Updated: 2025/05/05 00:31:09 by malrifai         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	perror_and_exit(const char *message)
{
	perror(message);
	exit(EXIT_FAILURE);
}

void	handle_prev_fd(int prev_fd)
{
	if (prev_fd > STDERR_FILENO && prev_fd != -1)
	{
		if (dup2(prev_fd, STDIN_FILENO) < 0)
			perror_and_exit("dup2");
		close(prev_fd);
	}
}

int	handle_fork_error(int prev_fd, t_minishell *data)
{
	perror("fork");
	if (prev_fd > STDERR_FILENO)
		close(prev_fd);
	data->last_exit_status = 1;
	return (1);
}
