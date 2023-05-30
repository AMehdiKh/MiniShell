/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hahadiou <hahadiou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/29 23:54:37 by hahadiou          #+#    #+#             */
/*   Updated: 2023/05/30 05:52:46 by hahadiou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_lexer	*ft_redi_parser(t_lexer *lexer, int stdin)
{
	while (lexer && (lexer->type == R_FILE || lexer->type == W_A_FILE \
		|| lexer->type == W_T_FILE || lexer->type == HEREDOC))
	{
		if (lexer->type == R_FILE)
			ft_dup2(ft_open(lexer->word, O_RDONLY, 0), STDIN_FILENO);
		else if (lexer->type == W_A_FILE || lexer->type == W_T_FILE)
			ft_dup2(ft_file2(lexer), STDOUT_FILENO);
        else if (lexer->type == HEREDOC)
            ft_dup2(ft_heredoc(lexer, stdin), STDIN_FILENO);
		lexer = lexer->next;
	}
	return (lexer);
}

t_lexer	*ft_grp_shift(t_lexer *lexer)
{
	while (lexer && (lexer->type == R_FILE || lexer->type == W_A_FILE 
		|| lexer->type == W_T_FILE || lexer->type == CMD \
			|| lexer->type == BUILTIN || lexer->type == HEREDOC))
		lexer = lexer->next;
	if (lexer && lexer->type == PIPE)
		lexer = lexer->next;
	return (lexer);
}

static int ft_check_pipe(t_lexer *lexer)
{
    while (lexer)
    {
        if (lexer->type == PIPE)
            return (1);
        lexer = lexer->next;
    }
    return (0);
}

static int ft_check_builtin(t_lexer *lexer)
{
    while (lexer)
    {
        if (lexer->type == BUILTIN)
            return (1);
        lexer = lexer->next;
    }
    return (0);
}

int	exec_builtin_child(t_lexer *lexer, t_shell *shell)
{
	char **argv;
	
	argv = ft_split_cmd(lexer->word);
    if (!ft_strncmp(argv[0], "echo", 5))
        return (ft_echo_builtin(ft_count_strings(argv), argv));
    else if (!ft_strncmp(argv[0], "pwd", 4))
        return (ft_pwd_builtin(shell));
    else if (!ft_strncmp(argv[0], "env", 4))
        return (ft_env_builtin(argv, shell));
	else if (!ft_strncmp(argv[0], "cd", 3))
        return (ft_cd_builtin(argv[1], shell));
	else if (!ft_strncmp(argv[0], "unset", 6))
        return (ft_unset_builtin((argv[1]), shell->env));
	else if (!ft_strncmp(argv[0], "exit", 5))
		ft_exit_builtin(ft_count_strings(argv), argv[1]);
	return (0);
}

void	setup_child(t_lexer *lexer, t_parser *pipex, t_shell *shell)
{
	t_lexer *cmd;
	int stdin;

	cmd = NULL;
	stdin = dup(0);
	close(pipex->pipefd[0]);
	ft_dup2(pipex->prev_in, 0);
	if (ft_check_pipe(lexer))
		ft_dup2(pipex->pipefd[1], 1);
	lexer = ft_redi_parser(lexer, stdin);
	if (!lexer)
		exit(EXIT_SUCCESS);
	if (lexer->type == CMD || lexer->type == BUILTIN)
	{
		cmd = lexer;
		lexer = lexer->next;
		lexer = ft_redi_parser(lexer, stdin);
	}
	if (cmd)
	{
		if (cmd->type == CMD)
			ft_check_cmd(cmd->word, pipex);
		else
			exit(exec_builtin_child(cmd, shell));
	}
}

void	ft_parser(t_parser *pipex, t_shell *shell, t_lexer *lexer)
{
	pid_t	pid;
	int		stdin;
	int		stdout;
	
	pipex->env = shell->env;
	stdin = dup(0);
	stdout = dup(1);
	if (!ft_check_pipe(lexer) && ft_check_builtin(lexer))
	{
		lexer = ft_redi_parser(lexer, stdin);
		t_lexer *cmd = lexer;
		lexer = lexer->next;
		lexer = ft_redi_parser(lexer, stdin);
		exec_builtin_child(cmd, shell);
	}
	ft_dup2(stdin, 0);
	ft_dup2(stdout, 1);
	ft_pipe(pipex);
	pid = ft_fork(pipex);
	if (!pid)
		setup_child(lexer, pipex, shell);
	ft_close_pipe(pipex);
	lexer = ft_grp_shift(lexer);
	if (lexer)
		ft_parser(pipex, shell, lexer);
	if (!lexer)
		waitpid(pid, &shell->exit_status, 0);
	else
		waitpid(pid, NULL, 0);
}