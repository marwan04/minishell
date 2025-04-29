/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: malrifai <malrifai@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/03 10:13:43 by eaqrabaw          #+#    #+#             */
/*   Updated: 2025/04/29 10:47:55 by malrifai         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H
# define _GNU_SOURCE
# include "libft/includes/libft.h"
# include <signal.h>
# include <sys/signal.h>
# include <stdio.h>
# include <readline/history.h>
# include <readline/readline.h>
# include <stdlib.h>
# include <sys/wait.h>
# include <dirent.h>
# include <fnmatch.h>
# include <sys/types.h>
# include <bits/sigaction.h>

#define HEREDOC_INTERRUPTED_SIG 1
#define HEREDOC_EOF 2
#define HEREDOC_SUCCESS 0

extern volatile sig_atomic_t g_sig_int;

typedef enum e_token_type
{
	APPEND,
	HEREDOC,
	PIPE,
	REDIR_IN,
	REDIR_OUT,
	OR,
	AND,
	LPAREN,
	RPAREN,
	WORD
}					t_token_type;

typedef enum e_node_type
{
	NODE_CMD,
	NODE_PIPE,
	NODE_REDIR_IN,
	NODE_REDIR_OUT,
	NODE_APPEND,
	NODE_HEREDOC,
	NODE_AND,
	NODE_OR,
	NODE_GROUP,
}					t_node_type;

typedef struct s_ast
{
	t_node_type		type;
	char			**args;
	char			*file;
	int 			heredoc_pipe[2];
	int				heredoc_expand;
	struct s_ast	*left;
	struct s_ast	*right;
}	t_ast;

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
	int				skip_env_lookup;
}					t_expand;

typedef struct s_token
{
	char			*value;
	struct s_token	*next;
	int				herdoc_quote;
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
	t_ast	*ast_root;
	t_env	*env;
	t_token	*tokens;
	int		execution_aborted;
}					t_minishell;

//tokenizing/check.c
int					check_quote(int *i, char *input, t_token **head);
int					check_redirections(int *i, char *input, t_token **head,
						char symbol[2]);
int					handle_words(int *i, char *input, t_token **head);
void				check_separator(int *i, char *input);

//tokenizing/cmd_utils.c
t_ast				*new_ast_cmd(void);
void				add_argument(t_ast *node, char *arg);
t_ast				*handle_redirection(t_ast *cmd_node, t_token *token);
t_ast				*parse_ast(t_token **tokens);

//tokenizing/free.c
void				free_tokens(t_minishell *data);
void				ft_free(t_minishell *data, int flag, char *msg);
void				free_ast(t_ast *node);

//tokenizing/parsing_utils.c
t_ast				*parse_command(t_token **tokens);
t_ast 				*parse_group(t_token **tokens);
t_ast				*parse_pipeline(t_token **tokens);
t_ast				*parse_and(t_token **tokens);
t_ast				*parse_or(t_token **tokens);
t_ast 				*parse_expression(t_token **tokens);

//tokenizing/tokenizer_utils.c
t_token				*last_token(t_token *token);
t_token				*new_token(char *value, t_token_type type);
void				add_token(t_token **head, char *value, t_token_type type);
void				remove_last_token(t_token **head);
void 				delete_next_token(t_token *prev_token);

//tokenizing/tokenizer.c
t_token				*tokenizer(char *input);

//expander/expand.c
char				*expand_line(char *line, t_minishell *data);
char				*expand_tilde(char *token);
char				*remove_quotes(char *input);
void				expand_tokens(t_token *tokens,
						int last_exit_status, t_env *env);

// expander/utils.c
char				*ft_strjoin_free(char *s1, char *s2);

// expander/var_expand.c
char				*expand_extract_var(t_expand *expand,
						char *token, int last_exit_status);
char				*expand_replace_var(char *var_name,
						int preserve_spaces, t_env *env);
char				*expand_variables(char *token,
						int last_exit_status, t_env *env);
void				expand_append_char(t_expand *expand, char c);
void				expand_track_quotes(t_expand *expand, char c);

// signal/signal_handler.c
void				handle_sigint(int sig);
void 				init_signals(void);
int				check_signal(t_minishell *data);

// builtins/echo.c
int					is_n_flag(char *arg);
void				handle_echo(char **args);

// builtins/unset.c
void				handle_unset(char **args, t_env **env);

// builtins/export/export.c
void				handle_export(char **args, t_env **env);
int					is_valid_identifier(char *arg);

// builtins/export/print_env.c
void				print_env_sorted(t_env *env);

// builtins/export/copy_env.c
t_env				*copy_env(t_env *original);
void				append_env_node(t_env **head, t_env **tail, t_env *new_node);
t_env				*create_env_node(t_env *src);

// builtins/cd.c
void				handle_cd(char **args, t_env **env);

// builtins/pwd.c
void				handle_pwd(void);

// builtins/exit.c
void				ft_exit(t_minishell *data);

// builtins/env.c
void				handle_env(t_env *env);
char				**build_env(t_env *env);
char				*get_env_value(t_env *env, char *key);
void				add_or_update_env(t_env **env, char *key, char *value);

// builtins/env_utils.c
char				**ft_free_env_array(char **env_array, int last);
t_env				*init_env_list(char **envp);
void				free_env(t_env *env);
void				free_env_node(t_env *node);

// builtins/export/export.c
void				handle_export(char **args, t_env **env);

// builtins/update_env_cd.c
int					update_pwd_env(t_env **env);


// exec/exec.c
int					ft_execute_command(t_ast *node, t_minishell *data);
void				execute_builtin_cmds(t_ast *node,
						int *last_exit_status, t_env **env);
int					exec_ast(t_ast *node, int prev_fd, t_minishell *data);

// exec/path.c
char				*ft_get_path(char *s, t_env **envp);

// exec/exec_herdoc.c

int					handle_heredoc_node(t_ast *node, int prev_fd, t_minishell *data);

// exec/read.c
void				ft_read(t_minishell *data);

// exec/error_utilites.c
void				close_on_exit(int *fds, int fd_count);
void				ft_perror(char *msg, int errno);
void				ft_set_exit_status(int *ptr, int status);

// exec/exec_utilites.c
int					initialize_execution_params(char **full_path,
						char ***envp, char **args, t_env **env);
int					is_builtin(char *cmd);

// exec/exec_pipes.c
int					handle_pipe_node(t_ast *node,
						int prev_fd, t_minishell *data);
int					handle_redirection_node(t_ast *node,
						int prev_fd, t_minishell *data);

// herdoc/herdoc_handler.c

void				collect_heredocs(t_ast *node, t_minishell *data);

// syntax/syntax_check.c
int					validate_token_sequence(t_token *tokens);
int					syntax_error(char *input);

// syntax/syntax_utils.c
int					starts_with_pipe_or_logical(char *input);
int					has_unmatched_quotes(char *input);
int					has_empty_parentheses(char *input);
int 				has_unmatched_parentheses(const char *input);
int					has_invalid_operator_sequence(char *input);

// // Testing
void				print_ast(t_ast *node, int depth, int is_left);
void				print_tokens(t_token *head);
void				test_heredoc_node(t_ast *node);
void 				generate_ast_diagram(t_ast *root);

// normalize_tokens.c
void normalize_tokens(t_token **tokens);
int is_redirection(t_token *token);
int is_pipe_or_logical(t_token *token);
void normalize_tokens_with_heredoc(t_token **tokens);
void 				expand_wildcards(t_token *tokens);

#endif
