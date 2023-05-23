/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hahadiou <hahadiou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/04 19:44:45 by hahadiou          #+#    #+#             */
/*   Updated: 2023/05/23 22:00:12 by hahadiou         ###   ########.fr       */
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
#include <fcntl.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <readline/readline.h>
#include <readline/history.h>
#include "lexer.h"
#include "parser.h"
#include "libft.h"

typedef	struct s_shell	t_shell;
typedef	struct	s_cmd	t_cmd;
typedef struct s_data	t_data;

struct	s_data
{
	char	*abs_cmd;
	char	**cmd_path;
	char	**argv;
	t_token	lr_op;
	t_token	rr_op;
	char	*lr;
	char	*rr;
};

struct	s_cmd
{
	t_token	type;  // PIPE etc
	t_token	lr_op; // </<<
	t_token	rr_op; // >/>>
	char	*lr;
	char	*rr;
	char	*cmd;
	char	**argv;
	t_cmd	*next;
};

struct s_shell
{
	t_lexer		*lexer;
	t_list		*list;
	t_cmd		*cmd;
	t_data		data;
	char		**env;
	char		*line;
	int			lexer_status;
	int			exit_status;
};

/*
	BUILTINS
*/
void	ft_setenv(t_shell *shell, char *variable, char *value);
int		ft_cd_builtin(char	*path, t_shell *shell);
int		ft_echo_builtin(size_t ac, char **av);
int		ft_env_builtin(char **av, t_shell *shell);
void	ft_exit_builtin(char *status, char *av, int ac);
int		ft_export_builtin(t_shell *shell, char **av);
int		ft_pwd_builtin(t_shell *shell);
int		ft_unset_builtin(char *var, char **envp);

/*
    LEXER
*/
void	ft_lexer_pipe(t_shell *, t_lexer *);
void	ft_read_redi(t_shell *, t_lexer *);
void	ft_write_redi(t_shell *shell, t_lexer *);
void	ft_lexer_cmd(t_shell *shell, t_lexer *);
char	*ft_syntax_err(t_list *list, int pipe);
void	ft_newnode(t_shell *node);
int		ft_check_meta(t_shell *shell);
int		ft_lexer(t_shell *shell);

t_list	*ft_split_list(const char *s);
char	*ft_remove_quotes(char *s, int);
int		ft_not_token(const char *s);
int		ft_quotes_number(char *s);

char	**ft_split(char const *s, char c, int option);

void printLexer(t_lexer* lexer);
void printList(t_list* list);
void print_cmd_node(t_cmd *node);
char    *get_last_file_word(t_lexer *head, t_token type[2]);


void    ft_cmdadd_back(t_cmd **lst, t_cmd *new);
void	ft_cmd_clear(t_cmd **lst);
void	heredoc(t_shell *shell, char *, bool);

/*
    EXPANSION
*/
char    *ft_getenv(char *var, char **envp, int n);
char	*expander(char *input, t_shell *shell);


/*
    PARSER
*/
void	parser(t_shell *shell);
t_cmd	*ft_newnode_cmd(t_token , t_data *);
t_cmd	*ft_newnode_pipe(t_token type);

/*
	EXECUTION
*/
void	executer(t_shell *shell, int io[2]);
void 	print_token(t_token token);
void 	mini_print_token(t_token token);

#endif
