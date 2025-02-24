/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error_utilites.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eaqrabaw <eaqrabaw@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/24 09:22:11 by eaqrabaw          #+#    #+#             */
/*   Updated: 2025/02/24 10:09:30 by eaqrabaw         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	ft_perror(char *msg, int errno)
{
	if (errno == 4242)
	{
		ft_putstr_fd(msg, 2);
		exit(1);
	}
	else
	{
		perror(msg);
		exit(errno);
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
