/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   write_heredoc.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eaqrabaw <eaqrabaw@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/01 19:06:00 by eaqrabaw          #+#    #+#             */
/*   Updated: 2025/05/01 19:06:00 by eaqrabaw         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void write_heredoc(char *delimiter, int pipe_fd)
{
    char *line = NULL;
    size_t len = 0;
    ssize_t read;
    char *input = NULL;
    size_t input_len = 0;

    while (1)
    {
        // Read input line
        printf("> ");
        read = getline(&line, &len, stdin);
        if (read == -1)
            break;

        // Remove trailing newline
        if (line[read - 1] == '\n')
            line[read - 1] = '\0';

        // Check if we reached the delimiter
        if (ft_strcmp(line, delimiter) == 0)
            break;

        // Write the line to the pipe
        write(pipe_fd, line, ft_strlen(line));
        write(pipe_fd, "\n", 1);
    }

    // Clean up
    if (line)
        free(line);
}
