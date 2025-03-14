# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: malrifai <malrifai@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/02/03 10:28:41 by eaqrabaw          #+#    #+#              #
#    Updated: 2025/03/14 15:01:46 by malrifai         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME    	= minishell
CC      	= cc
CFLAGS  	= -Wall -Wextra -Werror
incDir 		= includes
libftDir 	= $(incDir)/libft
objDir 		= obj
GREEN   	= \033[0;32m
RED    		= \033[0;31m
RESET   	= \033[0m
ARROW   	= ✔
BUILTINS	=	

EXEC		= exec/exec.c exec/path.c exec/exec_utilites.c exec/error_utilites.c

EXPANDER	= expander/expand.c expander/utils.c expander/var_expand.c

PARSING		=

BUILTINS	= builtins/echo.c builtins/unset.c builtins/export/export.c builtins/export/print_env.c builtins/export/check.c builtins/pwd.c builtins/env.c builtins/env_utils.c builtins/cd.c

SIGNAL		= signal/signal_handler.c

TOKENIZING	= tokenizing/tokenizer.c \
			  tokenizing/check.c \
			  tokenizing/tokenizer_utils.c \
			  tokenizing/cmd_utils.c \
			  tokenizing/free.c\
			  tokenizing/parsing_utils.c

SRCS		= main.c \
			  $(TOKENIZING) \
			  $(EXPANDER) \
			  $(SIGNAL) \
			  $(BUILTINS) \
			  $(EXEC)

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
