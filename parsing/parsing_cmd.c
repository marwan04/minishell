/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_cmd.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eaqrabaw <eaqrabaw@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/07 10:44:25 by eaqrabaw          #+#    #+#             */
/*   Updated: 2025/05/07 10:44:42 by eaqrabaw         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	process_redirections_before(t_token **tokens,
	t_token *redir_tokens[128], int *redir_count)
{
	while (*tokens && ((*tokens)->type == REDIR_IN
			|| (*tokens)->type == REDIR_OUT || (*tokens)->type == APPEND
			|| (*tokens)->type == HEREDOC))
	{
		redir_tokens[*redir_count] = *tokens;
		(*redir_count)++;
		*tokens = (*tokens)->next->next;
	}
}

static void	process_words(t_token **tokens, t_ast *cmd)
{
	while (*tokens && (*tokens)->type == WORD)
	{
		add_argument(cmd, (*tokens)->value);
		*tokens = (*tokens)->next;
	}
}

t_ast	*parse_command(t_token **tokens)
{
	t_ast	*cmd;
	t_token	*redir_tokens[128];
	int		redir_count;
	int		i;

	redir_count = 0;
	if (*tokens && (*tokens)->type == LPAREN)
		return (parse_group(tokens));
	cmd = new_ast_cmd();
	process_redirections_before(tokens, redir_tokens, &redir_count);
	process_words(tokens, cmd);
	process_redirections_before(tokens, redir_tokens, &redir_count);
	i = redir_count - 1;
	while (i >= 0)
	{
		cmd = handle_redirection(cmd, redir_tokens[i]);
		i--;
	}
	return (cmd);
}
