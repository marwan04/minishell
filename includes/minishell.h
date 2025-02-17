/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: malrifai <malrifai@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/03 10:13:43 by eaqrabaw          #+#    #+#             */
/*   Updated: 2025/02/17 18:48:07 by malrifai         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "libft/includes/libft.h"
# include <readline/history.h>
# include <readline/readline.h>
# include <signal.h>
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

typedef struct s_expand
{
	char			quote;
	int				start;
	int				i;
	int				preserve_spaces;
	char			*expanded;
	char			*exit_status;
	char			*var_name;
	char			*var_value;
	char			c[2];
}					t_expand;

typedef struct s_token
{
	char			*value;
	struct s_token	*next;
	t_token_type	type;
}					t_token;

typedef struct s_minishell
{
	t_cmd			*cmds;
	t_token			*tokens;
}					t_minishell;

// tokenizing/check.c
int					check_quote(int *i, char *input, t_token **head);
int					check_redirections(int *i, char *input, t_token **head,
						char symbol[2]);
int					handle_words(int *i, char *input, t_token **head);

// tokenizing/cmd_utils.c
t_cmd				*new_cmd(void);
void				add_argument(t_cmd *cmd, char *arg);
void				handle_redirection(t_cmd *cmd, t_token *token);

// tokenizing/free.c
void				free_tokens(t_minishell *data);
void				free_cmd(t_cmd *cmd);
void				free_cmds(t_minishell *data);
void				ft_free(t_minishell *data, int flag, char *msg);
void				free_cmds_list(t_cmd *head);

// tokenizing/parsing_utils.c
void				handle_args_in_cmd(t_cmd *cmd, t_token **tokens);
t_cmd				*create_new_cmd(t_cmd **current);
t_cmd				*parse_tokens(t_token *tokens);

// tokenizing/tokenizer_utils.c
t_token				*new_token(char *value, t_token_type type);
t_token				*last_token(t_token *token);
void				add_token(t_token **head, char *value, t_token_type type);

// tokenizing/tokenizer.c
t_token				*tokenizer(char *input);

// expander/expand.c
char				*expand_tilde(char *token);
char				*remove_quotes(char *input);
void				expand_tokens(t_token *tokens, int last_exit_status);

// expander/utils.c
char				*ft_strjoin_free(char *s1, char *s2);

// expander/var_expand.c
void				expand_track_quotes(t_expand *expand, char c);
void				expand_append_char(t_expand *expand, char c);
char				*expand_extract_var(t_expand *expand, char *token,
						int last_exit_status);
char				*expand_replace_var(char *var_name, int preserve_spaces);
char				*expand_variables(char *token, int last_exit_status);

// signal/signal_handler.c
void				handle_sigint(int sig);
void				signals_handler(void);

#endif
