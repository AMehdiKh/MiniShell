/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   libft.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hahadiou <hahadiou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/15 07:54:13 by Ael-khel          #+#    #+#             */
/*   Updated: 2023/05/28 17:31:04 by hahadiou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LIBFT_H
# define LIBFT_H

#include "get_next_line.h"
#include "ft_dprintf.h"
#include "lexer.h"

typedef struct s_list	t_list;

struct s_list
{
	char			*content;
	struct s_list	*next;
};

size_t		ft_strlcpy(char *dst, const char *src, size_t size);
size_t		ft_strlcat(char *dst, const char *src, size_t size);
size_t		ft_strlen(const char *s);
size_t		ft_count_strings(char **strings);

char		*ft_strnstr(const char *haystack, const char *needle, size_t len);
char		*ft_strmapi(char const *s, char (*f)(unsigned int, char));
char		*ft_substr(char const *s, unsigned int start, size_t len);
char		*ft_strtrim(char const *s1, char const *set);
char		*ft_strjoin(char const *s1, char const *s2, int option);
char		*ft_strrchr(const char *s, int c);
char		*ft_strchr(const char *s, int c);
char		*ft_strdup(const char *s);
char		*ft_itoa(int n);
char		*ft_litoa(int n);

char		**ft_split(char const *s, char c, int option);
void		*ft_clear_split(char **ptr, size_t x);
char		**ft_dup_env(char **main_env);
char		**extend_env(char **envp, char *newstr);
int			find_var_in_envp(const char* var, char** env);
char		*ft_getenv(char *var, char **envp);

void		ft_striteri(char *s, void (*f)(unsigned int, char*));
void		*ft_memmove(void *dst, const void *src, size_t n);
void		*ft_memcpy(void *dst, const void *src, size_t n);
void		*ft_memchr(const void *s, int c, size_t n);
void		*ft_memset(void *s, int c, size_t len);
void		*ft_calloc(size_t count, size_t size);
void    	*ft_realloc(void* ptr, size_t size);
void		ft_putendl_fd(char *s, int fd);
void		ft_putchar_fd(char c, int fd);
void		ft_putstr_fd(char *s, int fd);
void		ft_putnbr_fd(int n, int fd);
void		ft_bzero(void *s, size_t n);

int			ft_strncmp(const char *s1, const char *s2, size_t n);
int			ft_memcmp(const void *s1, const void *s2, size_t n);
int			ft_atoi(const char *s);
int			ft_isalpha(int c);
int			ft_isdigit(int c);
int			ft_isalnum(int c);
int 		ft_isspace(int c);
int			ft_isascii(int c);
int			ft_isprint(int c);
int			ft_toupper(int c);
int			ft_tolower(int c);

t_list		*ft_lstmap(t_list *lst, void *(*f)(void *), void (*del)(void *));
t_list		*ft_lstnew(void *content);
t_list		*ft_lstlast(t_list *lst);
t_lexer		*ft_lexer_last(t_lexer *lst);

void		ft_lstdelone(t_list *lst);
void		ft_lstclear(t_list **lst);
void		ft_lstiter(t_list *lst, void (*f)(void *));
void		ft_lstadd_front(t_list **lst, t_list *new);
void		ft_lstadd_back(t_list **lst, t_list *new);
int			ft_lstsize(t_list *lst);
void		ft_lexer_clear(t_lexer **lexer);

#endif