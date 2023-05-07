# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: ael-khel <ael-khel@student.1337.ma>        +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/04/27 12:41:59 by ael-khel          #+#    #+#              #
#    Updated: 2023/04/30 06:26:45 by ael-khel         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

###############################################################################################################
NAME = MiniShell

BNAME = MiniShell_bonus
###############################################################################################################
CC = gcc

CFLAGS = -Wall -Wextra -MMD
###############################################################################################################
M_DIR = Mandatory

M_OBJ_DIR = Mandatory/objs

M_FILES = main 

M_SRCS = $(addsuffix .c,$(M_FILES))

M_OBJS = ${M_SRCS:%.c=$(M_OBJ_DIR)/%.o}

M_DEP = ${M_OBJS:.o=.d}
##############################################################################################################
B_DIR = Bonus

B_OBJ_DIR = Bonus/objs

B_FILES = main

B_SRCS = $(addsuffix _bonus.c,$(B_FILES))

B_OBJS = ${B_SRCS:%.c=$(B_OBJ_DIR)/%.o}

B_DEP = ${B_OBJS:.o=.d}
##############################################################################################################
LIBFT = LibFT/libft.a
READLINE = 
##############################################################################################################
.PHONY: clean
man: $(NAME)

$(NAME): $(LIBFT) $(M_OBJS)
	$(CC) $(M_OBJS) $(LIBFT) -lreadline -o $@

$(M_OBJ_DIR)/%.o: $(M_DIR)/%.c
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) -c $< -o $@

bonus: $(BNAME)

$(BNAME): $(LIBFT) $(B_OBJS)
	$(CC) $(B_OBJS) $(LIBFT) -o $@

$(B_OBJ_DIR)/%.o: $(B_DIR)/%.c
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) -c $< -o $@

all: man bonus

$(LIBFT):
	$(MAKE) -C LibFT

sinclude $(M_DEP) $(B_DEP)

clean:
	$(MAKE) fclean -C LibFT
	$(RM) -r $(M_OBJ_DIR) $(B_OBJ_DIR)

fclean: clean
	$(RM) $(NAME) $(BNAME)

re: fclean all
# $(CC) $(M_OBJS) $(LIBFT) -L/Users/ael-khel/homebrew/opt/readline/lib -lreadline -o $@
# $(CC) -I/Users/ael-khel/homebrew/opt/readline/include $(CFLAGS) -c $< -o $@
