/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ael-khel <ael-khel@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/29 23:54:37 by hahadiou          #+#    #+#             */
/*   Updated: 2023/06/15 15:11:41 by ael-khel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_lexer	*ft_redi_parser(t_shell *shell, t_lexer *lexer)
{
	while (lexer && (lexer->type == R_FILE || lexer->type == W_A_FILE
			|| lexer->type == W_T_FILE || lexer->type == HEREDOC))
	{
		if (lexer->type == R_FILE)
			ft_dup2(ft_open(lexer->word, O_RDONLY, 0), STDIN_FILENO);
		else if (lexer->type == W_A_FILE || lexer->type == W_T_FILE)
			ft_dup2(ft_file2(lexer), STDOUT_FILENO);
		else if (lexer->type == HEREDOC)
			ft_dup2(ft_heredoc(shell, lexer), STDIN_FILENO);
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

void	setup_child(t_lexer *lexer, t_parser *parser, \
		t_shell *shell, t_lexer *cmd)
{
	signal(SIGINT, child_signals);
	ft_dup2(parser->prev_in, 0);
	if (ft_check_pipe(lexer))
	{
		close(parser->pipefd[0]);
		ft_dup2(parser->pipefd[1], 1);
	}
	lexer = ft_redi_parser(shell, lexer);
	if (!lexer)
		exit(EXIT_SUCCESS);
	if (lexer->type == CMD || lexer->type == BUILTIN)
	{
		cmd = lexer;
		lexer = lexer->next;
		lexer = ft_redi_parser(shell, lexer);
	}
	if (cmd)
	{
		if (cmd->type == CMD)
			ft_check_cmd(cmd->word, parser);
		else
			exit(exec_builtin_child(cmd, shell));
	}
}

void	ft_compound_cmd(t_parser *parser, t_shell *shell, \
		t_lexer *lexer)
{
	pid_t	pid;

	if (ft_check_pipe(lexer))
		ft_pipe(parser);
	pid = ft_fork(parser);
	if (!pid)
		setup_child(lexer, parser, shell, NULL);
	if (ft_check_pipe(lexer))
		ft_close_pipe(parser);
	else if (parser->prev_in != 0)
		close(parser->prev_in);
	lexer = ft_grp_shift(lexer);
	if (lexer)
		ft_parser(parser, shell, lexer);
	if (!lexer)
	{
		waitpid(pid, &shell->exit_status, 0);
		shell->exit_status = shell->exit_status >> 8;
	}
	else
		waitpid(pid, NULL, 0);
}

void	ft_parser(t_parser *parser, t_shell *shell, t_lexer *lexer)
{
	t_lexer	*cmd;

	ft_save_std(parser);
	parser->env = shell->env;
	if (!ft_check_pipe(lexer) && ft_check_builtin(lexer))
	{
		lexer = ft_redi_parser(shell, lexer);
		cmd = lexer;
		lexer = lexer->next;
		lexer = ft_redi_parser(shell, lexer);
		exec_builtin_child(cmd, shell);
		ft_dup2(parser->_stdin, 0);
		ft_dup2(parser->_stdout, 1);
	}
	else
		ft_compound_cmd(parser, shell, lexer);
}
