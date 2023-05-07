# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: hahadiou <hahadiou@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/04/11 22:55:49 by hahadiou          #+#    #+#              #
#    Updated: 2023/04/14 00:30:09 by hahadiou         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

CC		= cc
FLAGS	= -Wall -Wextra -Werror

NAME	= lexer

OBJ_PATH	= obj

SRCS = lexer.c


NOC		= \033[0m
RED		= \033[1;31m
GREEN	= \033[1;32m
YELLOW	= \033[1;33m
BLUE	= \033[1;34m
WHITE	= \033[1;37m

all: $(NAME)


$(NAME):
	@echo "$(YELLOW)Compiling Lexer...$(NOC)"
	@$(CC) $(FLAGS) $(SRCS) -lreadline -o $@ $^ 
	@echo "$(GREEN)$@$(NOC)"

run:
	make re && ./lexer

clean:
	@echo "$(RED)Cleaning Up... ✔️$(NOC)"
	@rm -f $(NAME)

re: clean all

.PHONY:	all clean fclean re 
