/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hahadiou <hahadiou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/02 15:18:29 by hahadiou          #+#    #+#             */
/*   Updated: 2023/06/04 17:32:03 by hahadiou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

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
void					ft_setenv(t_shell *shell, char *s, char *e);
int						ft_cd_builtin(char *s, t_shell *shell);
int						ft_echo_builtin(size_t i, char **e);
void					ft_env_builtin(char **e, t_shell *s);
void					ft_exit_builtin(int i, char *s);
int						ft_export_builtin(t_shell *s, char **e);
int						ft_pwd_builtin(void);
int						ft_unset_builtin(char *s, char **e);

/*
	LEXER
*/
char					*ft_arg_join(t_shell *shell);
int						ft_builtin_check(char *s, t_shell *shell);
void					ft_lexer_pipe(t_shell *s, t_lexer *l);
void					ft_read_redi(t_shell *s, t_lexer *l);
void					ft_write_redi(t_shell *s, t_lexer *l);
void					ft_lexer_cmd(t_shell *s, t_lexer *l);
char					*ft_syntax_err(t_list *l, int i);
void					ft_newnode(t_shell *s);
int						ft_check_meta(t_shell *s);
int						ft_lexer(t_shell *s);
t_list					*ft_split_list(const char *s);
char					*ft_remove_quotes(char *s, int i);
int						ft_not_token(const char *s);
int						ft_quotes_number(char *s);

/*
	EXPANSION
*/
char					*ft_getenv(char *s, char **e);
char					*ft_expander(char *s, t_shell *sh);
int						expanded_size(char *input, t_shell *shell);
size_t					expand_size(char *input, size_t *i, t_shell *shell);
size_t					exit_status_size(t_shell *shell);
int						ft_heredoc(t_shell *shell, t_lexer *lexer, int stdin);
bool					ft_search(char *s, char c);
void					ft_parser(t_parser *p, t_shell *s, t_lexer *l, int *a);
void					ft_check_cmd(char *arg, t_parser *parser);
void					ft_cmds_parse(char *arg, t_parser *parser);
void					ft_parse_path(t_parser *parser);
void					ft_slash_end(t_parser *parser);
int						ft_open(const char *pathname, int flags, mode_t mode);
void					ft_pipe(t_parser *parser);
pid_t					ft_fork(t_parser *parser);
void					ft_dup2(int old, int new);
void					ft_execve(t_parser *parser);
int						ft_file2(t_lexer *l);
void					ft_close_pipe(t_parser *parser);
void					ft_clear(char **ptr);
char					**ft_split_cmd(char const *s);
void					config_signals(void);
void					dismiss_signal(int signum);
void					child_signals(int signum);
char					*get_var_name(const char *s, int c);
bool					is_valid_id(char *str);
bool					streq(char *str1, char *str2);
int						sirche(char *s);
int						ft_check_pipe(t_lexer *lexer);
int						ft_check_builtin(t_lexer *lexer);
char					*setup_red(t_token type, t_shell *shell);
void					ft_save_std(int std[2]);
int						ft_execute_builtin(t_shell *s, t_lexer *l, int se);
int						exec_builtin_child(t_lexer *lexer, t_shell *shell);
t_lexer					*ft_redi_parser(t_shell *s, t_lexer *l, int std);
#endif
