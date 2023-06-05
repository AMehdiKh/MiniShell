/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hahadiou <hahadiou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/29 23:54:37 by hahadiou          #+#    #+#             */
/*   Updated: 2023/06/04 18:06:22 by hahadiou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_lexer	*ft_redi_parser(t_shell *shell, t_lexer *lexer, int stdin)
{
	while (lexer && (lexer->type == R_FILE || lexer->type == W_A_FILE
			|| lexer->type == W_T_FILE || lexer->type == HEREDOC))
	{
		if (lexer->type == R_FILE)
			ft_dup2(ft_open(lexer->word, O_RDONLY, 0), STDIN_FILENO);
		else if (lexer->type == W_A_FILE || lexer->type == W_T_FILE)
			ft_dup2(ft_file2(lexer), STDOUT_FILENO);
		else if (lexer->type == HEREDOC)
			ft_dup2(ft_heredoc(shell, lexer, stdin), STDIN_FILENO);
		lexer = lexer->next;
	}
	return (lexer);
}

t_lexer	*ft_grp_shift(t_lexer *lexer)
{
	while (lexer && (lexer->type == R_FILE || lexer->type == W_A_FILE
			|| lexer->type == W_T_FILE || lexer->type == CMD
			|| lexer->type == BUILTIN || lexer->type == HEREDOC))
		lexer = lexer->next;
	if (lexer && lexer->type == PIPE)
		lexer = lexer->next;
	return (lexer);
}

int	exec_builtin_child(t_lexer *lexer, t_shell *shell)
{
	char	**argv;

	argv = ft_split_cmd(lexer->word);
	if (!ft_strncmp(argv[0], "echo", 5))
		return (ft_echo_builtin(ft_count_strings(argv), argv));
	else if (!ft_strncmp(argv[0], "pwd", 4))
		return (ft_pwd_builtin());
	else if (!ft_strncmp(argv[0], "env", 4))
		return (ft_env_builtin(argv, shell), -1);
	else if (!ft_strncmp(argv[0], "cd", 3))
		return (ft_cd_builtin(argv[1], shell), -1);
	else if (!ft_strncmp(argv[0], "unset", 6))
		return (ft_unset_builtin((argv[1]), shell->env));
	else if (!ft_strncmp(argv[0], "exit", 5))
		ft_exit_builtin(ft_count_strings(argv), argv[1]);
	else if (!ft_strncmp(argv[0], "export", 7))
		return (ft_export_builtin(shell, argv), -1);
	return (0);
}

void	setup_child(t_lexer *lexer, t_parser *parser, t_shell *shell)
{
	t_lexer	*cmd;
	int		stdin;

	signal(SIGINT, child_signals);
	cmd = NULL;
	stdin = dup(0);
	close(parser->pipefd[0]);
	ft_dup2(parser->prev_in, 0);
	if (ft_check_pipe(lexer))
		ft_dup2(parser->pipefd[1], 1);
	lexer = ft_redi_parser(shell, lexer, stdin);
	if (!lexer)
		exit(EXIT_SUCCESS);
	if (lexer->type == CMD || lexer->type == BUILTIN)
	{
		cmd = lexer;
		lexer = lexer->next;
		lexer = ft_redi_parser(shell, lexer, stdin);
	}
	if (cmd)
	{
		if (cmd->type == CMD)
			ft_check_cmd(cmd->word, parser);
		else
			exit(exec_builtin_child(cmd, shell));
	}
}

void	ft_parser(t_parser *parser, t_shell *shell, t_lexer *lexer, int std[2])
{
	pid_t	pid;
	t_lexer	*cmd;

	parser->env = shell->env;
	ft_save_std(std);
	if (!ft_check_pipe(lexer) && ft_check_builtin(lexer))
	{
		lexer = ft_redi_parser(shell, lexer, std[0]);
		cmd = lexer;
		lexer = lexer->next;
		lexer = ft_redi_parser(shell, lexer, std[0]);
		if (exec_builtin_child(cmd, shell) < 0)
			return ;
	}
	ft_dup2(std[0], 0);
	ft_dup2(std[1], 1);
	ft_pipe(parser);
	pid = ft_fork(parser);
	if (!pid)
		setup_child(lexer, parser, shell);
	ft_close_pipe(parser);
	lexer = ft_grp_shift(lexer);
	if (lexer)
		ft_parser(parser, shell, lexer, std);
	if (!lexer)
	{
		waitpid(pid, &shell->exit_status, 0);
		shell->exit_status = shell->exit_status >> 8;
	}
	else
		waitpid(pid, NULL, 0);
}
