/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   read_heredoc.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: malrifai <malrifai@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/05 14:29:09 by malrifai          #+#    #+#             */
/*   Updated: 2025/05/05 14:38:50 by malrifai         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	is_delim_or_null(char *line, char *delim)
{
	if (!line)
		return (1);
	if (ft_strcmp(line, delim) == 0)
	{
		free(line);
		return (1);
	}
	return (0);
}

static char	*expand_line_if_needed(char *line, int expand, t_minishell *data)
{
	char	*expanded;

	if (!expand)
		return (line);
	expanded = expand_vars(line, data->env);
	free(line);
	return (expanded);
}

static void	write_and_free_line(char *line, int fd)
{
	write(fd, line, ft_strlen(line));
	write(fd, "\n", 1);
	free(line);
}

void	read_heredoc(char *delim,
	int write_fd, int expand, t_minishell *data)
{
	char	*line;

	signal(SIGINT, SIG_DFL);
	while (1)
	{
		line = readline("> ");
		if (is_delim_or_null(line, delim))
			break ;
		line = expand_line_if_needed(line, expand, data);
		if (!line)
			continue ;
		write_and_free_line(line, write_fd);
	}
	close(write_fd);
}
