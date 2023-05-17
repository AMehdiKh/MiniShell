/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hahadiou <hahadiou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/04 19:44:45 by hahadiou          #+#    #+#             */
/*   Updated: 2023/05/17 14:23:25 by hahadiou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef minishell_H
# define minishell_H


#include <string.h>
#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>
#include <limits.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <readline/readline.h>
#include <readline/history.h>
#include "lexer.h"
#include "libft.h"

typedef	struct s_shell	t_shell;

struct s_shell
{
	t_lexer	*lexer;
	t_list	*list;
	char	**env;
	char	*line;
	int		lexer_status;
	int		exit_status;
};

/*
    LEXER
*/
void	ft_lexer_pipe(t_shell *shell);
void	ft_read_redi(t_shell *shell);
void	ft_write_redi(t_shell *shell);
char	*ft_syntax_err(t_list *list, int pipe);
void	ft_newnode(t_lexer **node);
int		ft_check_meta(t_shell *shell);
int		ft_lexer(t_shell *shell);

t_list	*ft_split_list(const char *s);
char	*ft_remove_quotes(char *s);
int		ft_not_token(const char *s);
int		ft_quotes_number(char *s);

char	**ft_split(char const *s, char c, int option);

void printLexer(t_lexer* lexer);
void printList(t_list* list);

/*
    EXPANSION
*/
char    *ft_getenv(char *var, char **envp, int n);
char	*expander(char *input, t_shell *shell);


/*
	BUILTIN
*/
int		ft_env(char **av, t_shell *shell);


/*
    PARSER
*/

#endif
