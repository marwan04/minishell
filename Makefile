# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: malrifai <malrifai@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/02/03 10:28:41 by eaqrabaw          #+#    #+#              #
#    Updated: 2025/05/05 14:38:05 by malrifai         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME    	= minishell
CC      	= cc
incDir 		= includes
CFLAGS  	= -Wall -Wextra -Werror -I$(incDir) -g
libftDir 	= $(incDir)/libft
objDir 		= obj
GREEN   	= \033[0;32m
RED    		= \033[0;31m
RESET   	= \033[0m
ARROW   	= ✔

EXEC		= exec/exec_and_or.c \
			  exec/pipeline_command.c \
			  exec/pipeline_fork.c \
			  exec/pipeline_redirect.c \
			  exec/pipeline_utils.c \
			  exec/exec_builtins.c \
			  exec/path.c \
			  exec/check_access.c \
			  exec/error_utilites.c \
			  exec/exec_redirection.c \
			  exec/pipe_utils.c \
			  exec/exec.c \
			  exec/exec_cmd.c \
			  exec/exec_pipe.c \
			  exec/exec_utils.c
			  
EXPANDER	= expander/expand_wildcard.c \
			  expander/replace_var.c \
			  expander/expand_special.c \
			  expander/expand.c \
			  expander/utils.c \
			  expander/var_expand.c \
			  expander/var_expand_utils.c

BUILTINS	= builtins/exit.c \
			  builtins/update_env_cd.c \
			  builtins/update_env.c \
			  builtins/echo.c \
			  builtins/unset.c \
			  builtins/export/export.c \
			  builtins/export/copy_env.c \
			  builtins/export/print_env.c \
			  builtins/pwd.c \
			  builtins/env.c \
			  builtins/env_utils.c \
			  builtins/cd.c

SIGNAL		= signal/signal_handler.c

READ 		= read/read.c \
			  read/read_process.c

HERDOC 		= herdoc/herdoc_handler.c \
			  herdoc/herdoc_utils.c \
			  herdoc/read_heredoc.c

SYNTAX		= syntax/syntax_check.c \
			  syntax/syntax_utils.c

TOKENIZING	= tokenizing/tokenizer.c \
			  tokenizing/init_nodes.c \
			  tokenizing/normalize_helpers.c \
			  tokenizing/check.c \
			  tokenizing/tokenizer_utils.c \
			  tokenizing/cmd_utils.c \
			  tokenizing/free.c\
			  tokenizing/parsing_utils.c \
			  tokenizing/normalize_tokens.c

SRCS		= main.c \
			  $(TOKENIZING) \
			  $(EXPANDER) \
			  $(SIGNAL) \
			  $(BUILTINS) \
			  $(HERDOC) \
			  $(EXEC) \
			  $(READ) \
			  $(SYNTAX)

OBJS		= $(SRCS:%.c=$(objDir)/%.o)

all: $(NAME)

$(NAME): $(OBJS) $(libftDir)/libft.a
	@echo "$(GREEN)Making $(NAME)...$(RESET)"
	@$(CC) -o $(NAME) $(OBJS) -L$(libftDir) -lft -lreadline -lncurses
	@echo "$(GREEN)Done $(ARROW)$(RESET)"
	
$(objDir)/%.o: %.c
	@mkdir -p $(dir $@)
	@$(CC) $(CFLAGS) -I$(incDir) -c $< -o $@

$(libftDir)/libft.a:
	@$(MAKE) -C $(libftDir)

clean:
	@echo "$(RED)Deleting $(objDir)...$(RESET)"
	@$(MAKE) -C $(libftDir) clean
	@rm -rf $(objDir)
	@echo "$(RED)Done $(ARROW)$(RESET)"

fclean: clean
	@echo "$(RED)Deleting $(NAME)...$(RESET)"
	@$(MAKE) -C $(libftDir) fclean
	@rm -f $(NAME)
	@echo "$(RED)Done $(ARROW)$(RESET)"

re: fclean all

.PHONY: all clean fclean re
