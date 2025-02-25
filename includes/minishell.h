/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eaqrabaw <eaqrabaw@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/03 10:13:43 by eaqrabaw          #+#    #+#             */
/*   Updated: 2025/02/25 08:32:34 by eaqrabaw         ###   ########.fr       */
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
# include <sys/wait.h>

typedef enum e_token_type
{
	APPEND,
	HEREDOC,
	PIPE,
	REDIR_IN,
	REDIR_OUT,
	WORD
}					t_token_type;

typedef struct s_cmd
{
	char			**args;
	char			*input;
	char			*output;
	int				has_redirection;
	int				append;
	int				pipe;
	struct s_cmd	*next;
}					t_cmd;

typedef struct s_expand
{
	char			*exit_status;
	char			*expanded;
	char			*var_name;
	char			*var_value;
	char			c[2];
	char			quote;
	int				i;
	int				preserve_spaces;
	int				start;
}					t_expand;

typedef struct s_token
{
	char			*value;
	struct s_token	*next;
	t_token_type	type;
}					t_token;

typedef struct s_env
{
	char			*key;
	char			*value;
	struct s_env	*next;
}					t_env;

typedef struct s_minishell
{
	int		last_exit_status;
	t_cmd	*cmds;
	t_env	*env;
	t_token	*tokens;
}					t_minishell;

// tokenizing/check.c
int					check_quote(int *i, char *input, t_token **head);
int					check_redirections(int *i, char *input, t_token **head, char symbol[2]);
int					handle_words(int *i, char *input, t_token **head);
void				check_separator(int *i, char *input);

// tokenizing/cmd_utils.c
t_cmd				*new_cmd(void);
void				add_argument(t_cmd *cmd, char *arg);
void				handle_redirection(t_cmd *cmd, t_token *token);

// tokenizing/free.c
void				free_cmd(t_cmd *cmd);
void				free_cmds(t_minishell *data);
void				free_cmds_list(t_cmd *head);
void				free_tokens(t_minishell *data);
void				ft_free(t_minishell *data, int flag, char *msg);

// tokenizing/parsing_utils.c
t_cmd				*create_new_cmd(t_cmd **current);
t_cmd				*parse_tokens(t_token *tokens);
void				handle_args_in_cmd(t_cmd *cmd, t_token **tokens);

// tokenizing/tokenizer_utils.c
t_token				*last_token(t_token *token);
t_token				*new_token(char *value, t_token_type type);
void				add_token(t_token **head, char *value, t_token_type type);
void				remove_last_token(t_token **head);

// tokenizing/tokenizer.c
t_token				*tokenizer(char *input);

// expander/expand.c
char				*expand_tilde(char *token);
char				*remove_quotes(char *input);
void				expand_tokens(t_token *tokens, int last_exit_status, t_env *env);

// expander/utils.c
char				*ft_strjoin_free(char *s1, char *s2);

// expander/var_expand.c
char				*expand_extract_var(t_expand *expand, char *token, int last_exit_status);
char				*expand_replace_var(char *var_name, int preserve_spaces, t_env *env);
char				*expand_variables(char *token, int last_exit_status, t_env *env);
void				expand_append_char(t_expand *expand, char c);
void				expand_track_quotes(t_expand *expand, char c);

// signal/signal_handler.c
void				handle_sigint(int sig);
void				signals_handler(void);

// builtins/echo.c
int					is_n_flag(char *arg);
void				handle_echo(char **args);

// builtins/export.c
void				handle_export(char **args, t_env **env);

// builtins/pwd.c
void				handle_pwd(void);

// builtins/env.c
void				handle_env(t_env *env);

// builtins/env_utils.c
char				**build_env(t_env *env);
char				*get_env_value(t_env *env, char *key);
t_env				*init_env_list(char **envp);
void				add_or_update_env(t_env **env, char *key, char *value);
void				free_env(t_env *env);
void				handle_export(char **args, t_env **env);

// builtins/pwd.c
void				handle_pwd(void);

// builtins/env.c
void				handle_env(t_env *env);

// builtins/env_utils.c
char				**build_env(t_env *env);
char				*get_env_value(t_env *env, char *key);
t_env				*init_env_list(char **envp);
void				add_or_update_env(t_env **env, char *key, char *value);
void				free_env(t_env *env);

// exec/exec.c
int 				ft_execute_command(t_cmd *cmds, int *last_exit_status, t_env **env);						
void				execute_builtin_cmds(t_cmd *cmds, int *last_exit_status, t_env **env);
void    			ft_execute(t_cmd *cmds, int *last_exit_status, t_env **env);
// exec/path.c
char				*ft_get_path(char *s, t_env **envp);

// exec/error_utilites.c
void				close_on_exit(int *fds, int fd_count);
void				ft_perror(char *msg, int errno);
void    			ft_set_exit_status(int *ptr, int status);

// exec/exec_utilites.c
int 				initialize_execution_params(char **full_path, char ***envp, char **args, t_env **env);

#endif
