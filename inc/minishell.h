/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hahadiou <hahadiou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/04 19:44:45 by hahadiou          #+#    #+#             */
/*   Updated: 2023/05/30 18:53:38 by hahadiou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef minishell_H
# define minishell_H

# include "lexer.h"
# include "libft.h"
# include <errno.h>
# include <fcntl.h>
# include <limits.h>
# include <readline/history.h>
# include <readline/readline.h>
# include <signal.h>
# include <stdbool.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <sys/types.h>
# include <sys/wait.h>
# include <unistd.h>

typedef struct s_shell	t_shell;
typedef struct s_cmd	t_cmd;
typedef struct s_data	t_data;
typedef struct s_parser	t_parser;

t_shell					*shell;

struct					s_parser
{
	int					pipefd[2];
	int					prev_in;
	char				**av;
	char				**cmd;
	char				**path;
	char				**env;
	char				*path_cmd;
};

struct					s_shell
{
	t_lexer				*lexer;
	t_list				*list;
	t_parser			parser[1];
	char				**env;
	char				*line;
	int					lexer_status;
	int					exit_status;
};

/*
	BUILTINS
*/
void					ft_setenv(t_shell *, char *, char *);
int						ft_cd_builtin(char *, t_shell *);
int						ft_echo_builtin(size_t, char **);
int						ft_env_builtin(char **, t_shell *);
void					ft_exit_builtin(int, char *);
int						ft_export_builtin(t_shell *, char **);
int						ft_pwd_builtin(void);
int						ft_unset_builtin(char *, char **);

/*
	LEXER
*/
char					*ft_arg_join(t_shell *shell);
int						ft_builtin_check(char *s, t_shell *shell);
void					ft_lexer_pipe(t_shell *, t_lexer *);
void					ft_read_redi(t_shell *, t_lexer *);
void					ft_write_redi(t_shell *, t_lexer *);
void					ft_lexer_cmd(t_shell *, t_lexer *);
char					*ft_syntax_err(t_list *, int);
void					ft_newnode(t_shell *);
int						ft_check_meta(t_shell *);
int						ft_lexer(t_shell *);
t_list					*ft_split_list(const char *);
char					*ft_remove_quotes(char *, int);
int						ft_not_token(const char *);
int						ft_quotes_number(char *);

/*
	EXPANSION
*/
char					*ft_getenv(char *, char **);
char					*expander(char *, t_shell *);

/* ############# - main_bonus.c - ############# */
int						ft_heredoc(t_shell *shell, t_lexer *lexer, int stdin);
void					ft_parser(t_parser *parser, t_shell *shell,
							t_lexer *lexer);
void					ft_check_cmd(char *arg, t_parser *parser);

/* ########### - parse_cmd_bonus.c - ########### */
void					ft_cmds_parse(char *arg, t_parser *parser);
void					ft_parse_path(t_parser *parser);
void					ft_slash_end(t_parser *parser);

/* ############ - utils_I_bonus.c - ############ */
int						ft_open(const char *pathname, int flags, mode_t mode);
void					ft_pipe(t_parser *parser);
pid_t					ft_fork(t_parser *parser);
void					ft_dup2(int old, int new);
void					ft_execve(t_parser *parser);

/* ############ - utils_II_bonus.c - ############ */
int						ft_file2(t_lexer *);
void					ft_close_pipe(t_parser *parser);
void					ft_clear(char **ptr);
char					**ft_split_cmd(char const *s);

void					config_signals(void);
void					dismiss_signal(int signum);
void					child_signals(int signum);

char					*ft_expander(char *s, t_shell *shell);

#endif
