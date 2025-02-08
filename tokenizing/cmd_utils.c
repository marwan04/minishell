/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: malrifai <malrifai@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/08 14:07:09 by malrifai          #+#    #+#             */
/*   Updated: 2025/02/08 20:26:15 by malrifai         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

t_cmd	*new_cmd(void)
{
	t_cmd	*cmd;

	cmd = malloc(sizeof(*cmd));
	if (!cmd)
		return (NULL);
	cmd->args = NULL;
	cmd->input = NULL;
	cmd->output = NULL;
	cmd->append = 0;
	cmd->pipe = 0;
	cmd->next = NULL;
	return (cmd);
}

void	add_argument(t_cmd *cmd, char *arg)
{
	int		count;
	char	**new_args;
	int		i;

	i = 0;
	count = 0;
	while (cmd->args && cmd->args[count])
		count++;
	new_args = malloc(sizeof(char *) * (count + 2));
	if (!new_args)
		return ;
	while (i < count)
	{
		new_args[i] = cmd->args[i];
		i++;
	}
	new_args[count] = ft_strdup(arg);
	new_args[count + 1] = NULL;
	free(cmd->args);
	cmd->args = new_args;
}

void	handle_redirection(t_cmd *cmd, t_token *token)
{
	if (token->type == REDIR_IN)
	{
		if (cmd->input)
			free(cmd->input);
		cmd->input = ft_strdup(token->next->value);
	}
	else if (token->type == REDIR_OUT || token->type == APPEND)
	{
		if (cmd->output)
			free(cmd->output);
		cmd->output = ft_strdup(token->next->value);
		cmd->append = (token->type == APPEND);
	}
}
