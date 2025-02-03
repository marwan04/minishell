# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: eaqrabaw <eaqrabaw@student.42amman.com>    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/02/03 10:28:41 by eaqrabaw          #+#    #+#              #
#    Updated: 2025/02/03 11:15:42 by eaqrabaw         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = minishell
CC = cc
Flags = -Wall -Wextra -Werror
SRCS = main
srcDir = srcs/
incDir = includes/
objDir = obj/
libftDir = $(incDir)/libft
INCs = minishell.h
Srcs = $(addprefix $(srcDir), $(addsuffix .c, $(SRCS)))
OBJ = $(Srcs:$(srcDir)%.c=$(objDir)%.o)
INC = $(addprefix $(incDir), $(INCs))
GREEN = \033[0;32m
RED = \033[0;31m
RESET = \033[0m
ARROW = ✔

all: $(NAME)

$(NAME): $(OBJ) $(libftDir)/libft.a
	@echo "$(GREEN)Making $(NAME)...$(RESET)"
	@$(CC) $(Flags) -o $(NAME) $(OBJ) -L$(libftDir) -lft
	@echo "$(GREEN)Done $(ARROW)$(RESET)"

$(objDir)%.o: $(srcDir)%.c $(INC) | $(objDir)
	@$(CC) $(Flags) -c $< -o $@

$(objDir):
	@mkdir -p $(objDir)

clean:
	@echo "$(RED)Deleting $(objDir)...$(RESET)"
	@rm -rf $(objDir)
	@$(MAKE) -C $(libftDir) clean
	@echo "$(RED)Done $(ARROW)$(RESET)"

fclean: clean
	@echo "$(RED)Deleting $(NAME)...$(RESET)"
	@rm -f $(NAME)
	@$(MAKE) -C $(libftDir) fclean
	@echo "$(RED)Done $(ARROW)$(RESET)"

re: fclean all

$(libftDir)/libft.a:
	@$(MAKE) -C $(libftDir)

.PHONY: all clean fclean re