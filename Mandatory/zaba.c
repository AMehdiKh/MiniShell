/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   zaba.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hahadiou <hahadiou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/29 23:54:37 by hahadiou          #+#    #+#             */
/*   Updated: 2023/05/30 01:30:24 by hahadiou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// int ft_heredoc(t_lexer *lexer)
// {
// 	char	*heredoc;
//     int     pfds[2];

//     pipe(pfds);
// 	while (1)
// 	{
// 		heredoc = readline("> ");
// 		if (ft_strnstr(heredoc, lexer->word, ft_strlen(lexer->word) + 1))
// 			break ;
// 		ft_dprintf(pfds[1], "%s\n", heredoc);
// 		free(heredoc);
// 	}
//     free(heredoc);
//     close(pfds[1]);
//     return (pfds[0]);
// }

void	setup_heredoc(t_lexer *node)
{
	char	*line;
	int		pfds[2];

	pipe(pfds);
	while (-42)
	{
		line = readline("> ");
		if (ft_strnstr(line, node->word, ft_strlen(node->word)))
			break ;
		ft_dprintf(pfds[1], "%s\n", line);
	}
	close(pfds[1]);
	dup2(pfds[0], STDIN_FILENO);
	close(pfds[0]);
	free(line);
	exit (EXIT_SUCCESS);
}

t_lexer	*ft_redi_parser(t_lexer *lexer)
{
	while (lexer && (lexer->type == R_FILE || lexer->type == W_A_FILE || lexer->type == W_T_FILE || lexer->type == HEREDOC))
	{
		if (lexer->type == R_FILE)
			ft_dup2(ft_open(lexer->word, O_RDONLY, 0), STDIN_FILENO);
		else if (lexer->type == W_A_FILE || lexer->type == W_T_FILE)
			ft_dup2(ft_file2(lexer), STDOUT_FILENO);
        else if (lexer->type == HEREDOC && fork() == 0)
			setup_heredoc(lexer);
            // ft_dup2(ft_heredoc(lexer), STDIN_FILENO);
		lexer = lexer->next;
	}
	return (lexer);
}

t_lexer	*ft_grp_shift(t_lexer *lexer)
{
	while (lexer && (lexer->type == R_FILE || lexer->type == W_A_FILE 
		|| lexer->type == W_T_FILE || lexer->type == CMD || lexer->type == BUILTIN))
		lexer = lexer->next;
	if (lexer && lexer->type == PIPE)
		lexer = lexer->next;
	return (lexer);
}

int ft_check_pipe(t_lexer *lexer);
void	ft_pipex(t_pipex *pipex, t_shell *shell, t_lexer *lexer)
{
    pipex->env = shell->env;
	pid_t	pid;

	ft_pipe(pipex);
	pid = ft_fork(pipex);
	if (pid == 0)
	{
		t_lexer *cmd;
        int stdout;
	
		cmd = NULL;
        stdout = dup(1);
		close(pipex->pipefd[0]);
		ft_dup2(pipex->prev_in, 0);
        if (ft_check_pipe(lexer))
    		ft_dup2(pipex->pipefd[1], 1);
		lexer = ft_redi_parser(lexer);
		if (!lexer)
			exit(EXIT_SUCCESS);
		if (lexer->type == CMD || lexer->type == BUILTIN)
		{
			cmd = lexer;
			lexer = lexer->next;
			lexer = ft_redi_parser(lexer);
		}
		if (cmd)
        {
			ft_check_cmd(cmd->word, pipex);
        }
	}
	ft_close_pipe(pipex);
	lexer = ft_grp_shift(lexer);
	if (lexer)
		ft_pipex(pipex, shell, lexer);
	if (!lexer)
		waitpid(pid, &pipex->exit_code, 0);
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
