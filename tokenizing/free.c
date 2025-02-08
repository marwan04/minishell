/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: malrifai <malrifai@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/08 18:29:42 by malrifai          #+#    #+#             */
/*   Updated: 2025/02/08 20:49:29 by malrifai         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void free_tokens(t_minishell *data)
{
    t_token *tmp;
    t_token *tmp2;

    tmp = data->tokens;
    while (tmp != NULL)
    {
        tmp2 = tmp->next;
        if (tmp->value)  // Ensure we free the value
            free(tmp->value);
        free(tmp);
        tmp = tmp2;
    }
    data->tokens = NULL; // Avoid dangling pointer
}


void free_cmds(t_minishell *data)
{
    t_cmd 	*tmp;
	t_cmd 	*tmp2;
	int		i;

    tmp = data->cmds;
    while (tmp != NULL)
    {
		if (tmp->input)
			free(tmp->input);
		if (tmp->output)
			free(tmp->output);
		if (tmp->args)
		{
			i = 0;
			while (tmp->args[i] != NULL)
				free(tmp->args[i++]);
			free(tmp->args);
		}
		tmp2 = tmp->next;
		free(tmp);
		tmp = tmp2;
    }
	data->cmds = NULL;
}

void    ft_free(t_minishell *data, int flag, char *msg)
{
    if (data->tokens)
        free_tokens(data);
    if (data->cmds)
        free_cmds(data);
    if (flag)
        ft_putendl_fd(msg, 2);
    exit(flag);
}
