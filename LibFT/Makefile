# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: hahadiou <hahadiou@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/05/15 21:16:26 by hahadiou          #+#    #+#              #
#    Updated: 2023/06/04 15:12:46 by hahadiou         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

SRCS = ft_isalpha.c ft_isdigit.c ft_isalnum.c  ft_isspace.c \
		ft_strlen.c ft_memset.c ft_bzero.c ft_memcpy.c  \
		ft_strlcpy.c ft_strlcat.c ft_strchr.c \
		ft_strncmp.c ft_strnstr.c ft_litoa.c \
		ft_calloc.c ft_strdup.c ft_substr.c ft_strjoin.c ft_strtrim.c ft_split.c \
		ft_dprintf.c ft_dprintf_utils.c \
		get_next_line.c get_next_line_utils.c ft_dup_env.c ft_count_strings.c ft_clear_split.c \
		ft_lstadd_back.c ft_lstclear.c ft_lstdelone.c ft_lstnew.c ft_lstlast.c ft_lexerclear.c ft_extend_env.c \
		ft_var_in_envp.c ft_get_env.c ft_set_env.c


OBJS = ${addprefix obj/, ${SRCS:.c=.o}}
OBJ_PATH = obj

NAME = libft.a

CC = cc

CFLAGS = -Wall -Wextra -Werror -I../inc/

RM = rm -f

all: ${NAME}

obj/%.o: %.c
		@mkdir -p $(OBJ_PATH)
		@${CC} ${CFLAGS} -c $< -o $@

$(NAME): $(OBJS)
		@ar -rc $(NAME) $(OBJS)

clean:
	@rm -rf $(OBJ_PATH)

fclean: clean
	@rm -f $(NAME)

re: fclean all

.PHONY:	all clean fclean re
