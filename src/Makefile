# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: hahadiou <hahadiou@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/04/03 20:41:40 by hahadiou          #+#    #+#              #
#    Updated: 2023/04/11 14:42:10 by hahadiou         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #


CC		= cc
FLAGS	= -Wall -Wextra -Werror -IINC

NAME	= minishell

INC	= inc
UTILS_PATH	= utils
SRC_PATH	= src
LEXER_PATH	= src/lexer
LEXER_OBJB_PATH	 = lexer_obj
OBJ_PATH	= obj

SRCS = minishell.c

LEXER_SRCS = lexer.c

		
SRC		= $(addprefix $(SRC_PATH)/,$(SRCS))
SH_SRC 	= $(addprefix $(LEXER_PATH)/,$(LEXER_SRCS))
OBJ		= $(addprefix $(OBJ_PATH)/,$(SRCS:.c=.o))
LEXER_OBJ	= $(addprefix $(LEXER_OBJB_PATH)/,$(LEXER_SRCS:.c=.o))

NOC		= \033[0m
RED		= \033[1;31m
GREEN	= \033[1;32m
YELLOW	= \033[1;33m
BLUE	= \033[1;34m
WHITE	= \033[1;37m

all: $(NAME)


$(NAME): $(OBJ) $(LEXER_OBJ)
	@echo "$(YELLOW)Compiling Minishell...$(NOC)"
	@$(CC) $(FLAGS) -lreadline -o $@ $^ 
	@echo "$(GREEN)$@$(NOC)"

$(OBJ_PATH)/%.o: $(SRC_PATH)/%.c $(INC)/$(NAME).h
	@mkdir -p obj
	@$(CC) $(FLAGS) -I$(INC) -c -o $@ $<

$(LEXER_OBJB_PATH)/%.o: $(LEXER_PATH)/%.c $(INC)/$(NAME).h
	@mkdir -p lexer_obj
	@$(CC) $(FLAGS) -I$(INC) -c -o $@ $<

run:
	make re && ./minishell
	
clean:
	@echo "$(RED)Deleting OBJS ✔️ $(NOC)"
	@rm -rf $(OBJ_PATH)
	@rm -rf $(LEXER_OBJB_PATH)

fclean: clean
	@echo "$(RED)Deleting Binary ✔️$(NOC)"
	@rm -f $(NAME)

re: fclean all

.PHONY:	all clean fclean re 
