/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error_utilites.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: malrifai <malrifai@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/24 09:22:11 by eaqrabaw          #+#    #+#             */
/*   Updated: 2025/03/05 23:28:20 by malrifai         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_error(char *msg, int errnum)
{
	if (errnum == 4242)
	{
		ft_putstr_fd(msg, 2);
		exit(1);
	}
	else
	{
		perror(msg);
		exit(errnum);
	}
}

void	close_on_exit(int *fds, int fd_count)
{
	int	i;

	i = 0;
	while (i < fd_count)
	{
		if (fds[i] != -1)
			close(fds[i]);
		i++;
	}
}

void	ft_set_exit_status(int *ptr, int status)
{
	if (WIFEXITED(status))
		*ptr = WEXITSTATUS(status);
	else
		*ptr = 128 + WTERMSIG(status);
}
