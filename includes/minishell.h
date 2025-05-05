/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: malrifai <malrifai@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/03 10:13:43 by eaqrabaw          #+#    #+#             */
/*   Updated: 2025/05/05 18:44:08 by malrifai         ###   ########.fr       */
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

# define HEREDOC_INTERRUPTED_SIG 1
# define HEREDOC_EOF 2
# define HEREDOC_SUCCESS 0

extern volatile sig_atomic_t	g_sig_int;

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
	char			*heredoc_delim;
	int				heredoc_pipe[2];
	int				heredoc_expand;
	struct s_ast	*left;
	struct s_ast	*right;
}					t_ast;

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

typedef struct s_redir_state
{
	t_token	*list;
	t_token	*tail;
}	t_redir_state;

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
	int		pipes_count;
	int		execution_aborted;
}					t_minishell;

typedef struct s_pipeline
{
	int		(*pipes)[2];
	pid_t	*pids;
	int		n_stages;
	int		prev_fd;
}	t_pipeline;

typedef struct s_redirect_args
{
	t_ast	*stage;
	int		pipe_count;
	int		index;
	int		(*pipes)[2];
}	t_redirect_args;

typedef struct s_child_args
{
	t_ast			**stages;
	int				i;
	int				pipe_count;
	int				prev_fd;
	int				(*pipes)[2];
	t_minishell		*data;
}	t_child_args;

typedef struct s_fork_args
{
	t_ast			**stages;
	int				n_stages;
	int				pipe_count;
	int				prev_fd;
	int				(*pipes)[2];
	pid_t			*pids;
	t_minishell		*data;
}	t_fork_args;

typedef struct s_exec_pipeline_args
{
	t_ast			**stages;
	int				n_stages;
	int				prev_fd;
	t_minishell		*data;
}	t_exec_pipeline_args;

//herdoc/herdoc_utils.c
void				close_heredoc_in_node(t_ast *node);
void				close_heredoc_pipes_in_stages(t_ast **stages, int n_stages);

//exec/pipeline_utils
void				close_all_pipes(int pipe_count, int (*pipes)[2]);
int					create_pipes(int pipe_count, int (*pipes)[2]);
int					count_pipes(t_ast *node);

//exec/pipeline_command.c
void				exec_handle_command(t_ast *cmd, t_minishell *data);

//exec/pipeline_redirect.c
void				exec_setup_redirects(t_redirect_args *a);

//exec/pipeline_fork.c
int					fork_and_exec(t_fork_args *f);

//exec/exec_cmd.c
int					exec_cmd_node(t_ast *node, int prev_fd, t_minishell *data);

//exec/pipe_utils.c
t_ast				**collect_pipeline_stages(t_ast *pipe_root, int *out_n);

//exec/exec_builtins.c
void				execute_builtin_cmds(t_ast *node,
						int *last_exit, t_env **env);
int					is_builtin(char *cmd);
int					exec_builtin(t_ast *node, t_minishell *data, int *status);

// exec/exec.c
int					exec_ast(t_ast *node, int prev_fd, t_minishell *data);
char				**envp_to_array(t_env *env);

//tokenizing/check.c
int					check_quote(int *i, char *input, t_token **head);
int					check_redirections(int *i, char *input, t_token **head,
						char symbol[2]);
int					handle_words(int *i, char *input, t_token **head);
void				check_separator(int *i, char *input);
void				add_token(t_token **head, char *value, t_token_type type);

//tokenizing/cmd_utils.c
t_ast				*new_ast_cmd(void);
void				add_argument(t_ast *node, char *arg);
t_ast				*handle_redirection(t_ast *node, t_token *token);
t_ast				*parse_ast(t_token **tokens);

//tokenizing/free.c
void				free_tokens(t_minishell *data);
void				ft_free(t_minishell *data, int flag, char *msg);
void				free_ast(t_ast *node);
void				close_heredocs(t_ast *node);

//tokenizing/parsing_utils.c
t_ast				*parse_command(t_token **tokens);
t_ast				*parse_group(t_token **tokens);
t_ast				*parse_pipeline(t_token **tokens);
t_ast				*parse_and(t_token **tokens);
t_ast				*parse_or(t_token **tokens);
t_ast				*parse_expression(t_token **tokens);

//tokenizing/tokenizer_utils.c
t_token				*last_token(t_token *token);
void				remove_last_token(t_token **head);
t_token				*new_token(char *value, t_token_type type);

//tokenizing/tokenizer.c
t_token				*tokenizer(char *input);

//expander/expand.c
char				*expand_line(char *line, t_minishell *data);
char				*expand_tilde(char *token);
char				*remove_quotes(char *input);
void				expand_tokens(t_token *tokens,
						int last_exit_status, t_env *env);
void				ft_free_after_cmd(t_minishell *data);

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
char				*expand_vars(char *line, t_env *env);

// signal/signal_handler.c
void				handle_sigint(int sig);
void				init_signals(void);
int					check_signal(t_minishell *data);
void				def_sig(void);
void				ign_sig(void);

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
void				append_env_node(t_env **head,
						t_env **tail, t_env *new_node);
t_env				*create_env_node(t_env *src);

// builtins/cd.c
void				handle_cd(char **args, t_env **env);

// builtins/pwd.c
void				handle_pwd(void);

// builtins/exit.c
void				ft_exit(t_minishell *data);
int					handle_exit(char **args, int *last_exit);

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
void				set_exit_status_from_wait(int status, t_minishell *data);

// builtins/export/export.c
void				handle_export(char **args, t_env **env);

// builtins/update_env_cd.c
int					update_pwd_env(t_env **env);

//exec/exec_redirection.c 
int					apply_redirections(t_ast *node);
void				handle_redir_child(t_ast *node,
						int prev_fd, t_minishell *data);

// exec/path.c
char				*ft_get_path(char *s, t_env **envp);

// exec/check_access.c
int					open_redir_file(t_ast *node);

// exec/read.c
void				ft_read(t_minishell *data);

//exec/exec_and_or.c
int					exec_and_or(t_ast *node, int prev_fd, t_minishell *data);

// exec/error_utilites.c
void				perror_and_exit(const char *message);
int					handle_fork_error(int prev_fd, t_minishell *data);
void				handle_prev_fd(int prev_fd);

//herdoc/herdoc_handler.c
void				collect_heredocs(t_ast *node, t_minishell *data);
void				read_heredoc(char *delim, int write_fd,
						int expand, t_minishell *data);
void				heredoc_child(t_ast *node, t_minishell *data);

//exec/exec_pipes.c
int					exec_pipeline(t_ast **stages,
						int n_stages, int prev_fd, t_minishell *data);

// syntax/syntax_check.c
int					validate_token_sequence(t_token *tokens);
int					syntax_error(char *input);

// syntax/syntax_utils.c
int					starts_with_pipe_or_logical(char *input);
int					has_unmatched_quotes(char *input);
int					has_empty_parentheses(char *input);
int					has_unmatched_parentheses(const char *input);
int					has_invalid_operator_sequence(char *input);

// normalize_tokens.c
void				normalize_tokens(t_token **tokens);
int					is_redirection(t_token *token);
int					is_pipe_or_logical(t_token *token);
void				expand_one_token(t_token *tok,
						int last_exit_status, t_env *env);
t_ast				*new_pipe_node(t_ast *left, t_ast *right);
t_ast				*create_redir_node(t_ast *cmd_node, t_token *token);
void				expand_wildcards(t_token *tokens);
void				ft_process_input(t_minishell *data, char *input);
void				free_tokens_from_list(t_token *token);
#endif
