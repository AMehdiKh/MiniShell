/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   zaba.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ael-khel <ael-khel@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/29 23:54:37 by hahadiou          #+#    #+#             */
/*   Updated: 2023/05/30 02:51:22 by ael-khel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int ft_heredoc(t_lexer *lexer, int stdin)
{
	char	*heredoc;
    int     pfds[2];

    pipe(pfds);
	while (1)
	{
		ft_dprintf(2, "> ");
		heredoc = get_next_line(stdin);
		if (ft_strnstr(heredoc, lexer->word, ft_strlen(lexer->word)))
			break ;
		ft_dprintf(pfds[1], "%s", heredoc);
		free(heredoc);
	}
    free(heredoc);
    close(pfds[1]);
    return (pfds[0]);
}

t_lexer	*ft_redi_parser(t_lexer *lexer, int stdin)
{
	while (lexer && (lexer->type == R_FILE || lexer->type == W_A_FILE || lexer->type == W_T_FILE || lexer->type == HEREDOC))
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
		|| lexer->type == W_T_FILE || lexer->type == CMD || lexer->type == BUILTIN || lexer->type == HEREDOC))
		lexer = lexer->next;
	if (lexer && lexer->type == PIPE)
		lexer = lexer->next;
	return (lexer);
}

int ft_check_pipe(t_lexer *lexer);
void	ft_parser(t_parser *pipex, t_shell *shell, t_lexer *lexer)
{
	// printLexer(shell->lexer);
	pid_t	pid;
	int		stdin;
	
	pipex->env = shell->env;
	stdin = dup(0);
	ft_pipe(pipex);
	pid = ft_fork(pipex);
	if (pid == 0)
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
			ft_check_cmd(cmd->word, pipex);
        }
	}
	ft_close_pipe(pipex);
	lexer = ft_grp_shift(lexer);
	if (lexer)
		ft_parser(pipex, shell, lexer);
	if (!lexer)
		waitpid(pid, &shell->exit_status, 0);
	else
		waitpid(pid, NULL, 0);
}

int ft_check_pipe(t_lexer *lexer)
{
    while (lexer)
    {
        if (lexer->type == PIPE)
            return (1);
        lexer = lexer->next;
    }
    return (0);
}
