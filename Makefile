# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: ael-khel <ael-khel@student.1337.ma>        +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/04/27 12:41:59 by ael-khel          #+#    #+#              #
#    Updated: 2023/06/15 14:49:41 by ael-khel         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

CC		= cc
FLAGS	= -Wall -Wextra -Werror
RLFLGS	= -L/Users/ael-khel/homebrew/opt/readline/lib -lreadline
RLOFLGS	= -I/Users/ael-khel/homebrew/opt/readline/include

NAME	= minishell

INC	= inc
UTILS_PATH		= LibFT
MANDATORY_PATH	= Mandatory
SHARED_PATH		= builtins
BUI_OBJ_PATH	 = bui_obj
OBJ_PATH		= obj

SRCS = main.c \
		lexer.c \
		lexer_utils.c \
		ft_split_list.c \
		ft_split_cmd.c \
		expander.c \
		expander_utils.c \
		parser.c \
		parser_utils.c \
		utils_II.c \
		utils_I.c \
		parse_cmd.c \
		signals.c \

BUI_SRCS = cd.c \
			echo.c \
			env.c \
			exit.c \
			export.c \
			export_utils.c \
			pwd.c \
			unset.c

SRC		= $(addprefix $(MANDATORY_PATH)/,$(SRCS))
SH_SRC 	= $(addprefix $(SHARED_PATH)/,$(BUI_SRCS))
OBJ		= $(addprefix $(OBJ_PATH)/,$(SRCS:.c=.o))
SH_OBJ	= $(addprefix $(BUI_OBJ_PATH)/,$(BUI_SRCS:.c=.o))

NOC		= \033[0m
RED		= \033[1;31m
GREEN	= \033[1;32m
YELLOW	= \033[1;33m
BLUE	= \033[1;34m
WHITE	= \033[1;37m

all: $(NAME)

$(NAME): $(OBJ) $(SH_OBJ)
	@echo "$(YELLOW)Compiling Utils...$(NOC)"
	@make -sC $(UTILS_PATH)
	@echo "$(YELLOW)Compiling minishell...$(NOC)"
	@$(CC) $^ -L $(UTILS_PATH) -lft $(RLFLGS) -I$(INC) -o $@
	@echo "$(GREEN)$@$(NOC)"

$(OBJ_PATH)/%.o: $(MANDATORY_PATH)/%.c $(INC)/minishell.h $(INC)/lexer.h $(INC)/libft.h 
	@mkdir -p obj
	@$(CC) $(FLAGS) $(RLOFLGS) -I$(INC) -c -o $@ $<

$(BUI_OBJ_PATH)/%.o: $(SHARED_PATH)/%.c $(INC)/$(NAME).h
	@mkdir -p bui_obj
	@$(CC) $(FLAGS) $(RLOFLGS) -I$(INC) -c -o $@ $<

clean:
	@echo "$(RED)Deleting OBJS ✔️ $(NOC)"
	@make clean -sC $(UTILS_PATH)
	@rm -rf $(OBJ_PATH)
	@rm -rf $(BUI_OBJ_PATH)

fclean: clean
	@echo "$(RED)Deleting Binary ✔️$(NOC)"
	@make fclean -sC $(UTILS_PATH)
	@rm -f $(NAME)

re: fclean all

.PHONY:	all clean fclean re
