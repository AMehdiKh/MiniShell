/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hahadiou <hahadiou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/04 19:44:45 by hahadiou          #+#    #+#             */
/*   Updated: 2023/05/25 17:30:07 by hahadiou         ###   ########.fr       */
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
void	ft_setenv(t_shell *, char *, char *);
int		ft_cd_builtin(char	*, t_shell *);
int		ft_echo_builtin(size_t , char **, t_cmd *);
int		ft_env_builtin(char **, t_shell *);
void	ft_exit_builtin(char *, char *, int );
int		ft_export_builtin(t_shell *, char **);
int		ft_pwd_builtin();
int		ft_unset_builtin(char *, char **);

/*
    LEXER
*/
void	ft_lexer_pipe(t_shell *, t_lexer *);
void	ft_read_redi(t_shell *, t_lexer *);
void	ft_write_redi(t_shell *, t_lexer *);
void	ft_lexer_cmd(t_shell *, t_lexer *);
char	*ft_syntax_err(t_list *, int );
void	ft_newnode(t_shell *);
int		ft_check_meta(t_shell *);
int		ft_lexer(t_shell *);

t_list	*ft_split_list(const char *);
char	*ft_remove_quotes(char *, int);
int		ft_not_token(const char *);
int		ft_quotes_number(char *);

void printLexer(t_lexer* );
void printList(t_list* );
void print_cmd_node(t_cmd *);
char    *get_last_file_word(t_lexer *, t_token type[2]);


void    ft_cmdadd_back(t_cmd **, t_cmd *);
void	ft_cmd_clear(t_cmd **);
void	heredoc(t_shell *, char *, bool, int pipe);

/*
    EXPANSION
*/
char    *ft_getenv(char *, char **, int );
char	*expander(char *, t_shell *);


/*
    PARSER
*/
void	parser(t_shell *);
t_cmd	*ft_newnode_cmd(t_token , t_data *);
t_cmd	*ft_newnode_pipe(t_token);

/*
	EXECUTION
*/
void	executer(t_shell *, int io[2]);
void 	print_token(t_token);
void 	mini_print_token(t_token);
void	execute_commands(t_shell *, t_cmd*);

#endif
