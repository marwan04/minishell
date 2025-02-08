/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: malrifai <malrifai@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/03 10:13:43 by eaqrabaw          #+#    #+#             */
/*   Updated: 2025/02/08 14:16:08 by malrifai         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "libft/includes/libft.h"
# include <readline/history.h>
# include <readline/readline.h>
# include <stdio.h>
# include <stdlib.h>

typedef enum e_token_type
{
	WORD,
	PIPE,
	REDIR_IN,
	REDIR_OUT,
	APPEND,
	HEREDOC
}					t_token_type;

typedef struct s_cmd
{
	char			**args;
	char			*input;
	char			*output;
	int				append;
	int				pipe;
	struct s_cmd	*next;
}					t_cmd;

typedef struct s_token
{
	char			*value;
	t_token_type	type;
	struct s_token	*next;
}					t_token;

t_token				*tokenizer(char *input);
int					check_redirections(int *i, char *input, t_token **head,
						char symbol[2]);
void				add_token(t_token **head, char *value, t_token_type type);
t_token				*new_token(char *value, t_token_type type);
int					check_quote(int *i, char *input, t_token **head);
int					handle_words(int *i, char *input, t_token **head);
t_cmd				*parse_tokens(t_token *tokens);
t_cmd				*new_cmd(void);
void				add_argument(t_cmd *cmd, char *arg);
void				handle_redirection(t_cmd *cmd, t_token *token);

#endif
