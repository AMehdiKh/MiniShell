/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_II.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hahadiou <hahadiou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/09 19:32:45 by hahadiou          #+#    #+#             */
/*   Updated: 2023/05/30 05:01:02 by hahadiou         ###   ########.fr       */
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

int	ft_file2(t_lexer *lexer)
{
	int		fd;

	if (lexer->type == W_A_FILE)
		fd = ft_open(lexer->word, O_CREAT | O_APPEND | O_WRONLY, 0644);
	if (lexer->type == W_T_FILE)
		fd = ft_open(lexer->word, O_CREAT | O_TRUNC | O_WRONLY, 0644);
	return (fd);
}

void	ft_close_pipe(t_parser *pipex)
{
	if (pipex->prev_in)
		close(pipex->prev_in);
	pipex->prev_in = dup(pipex->pipefd[0]);
	close(pipex->pipefd[0]);
	close(pipex->pipefd[1]);
}

void	ft_clear(char **ptr)
{
	int	i;

	i = 0;
	if (ptr)
	{
		while (ptr[i])
			free(ptr[i++]);
		free(ptr);
	}
}

void	ft_newnode(t_shell *shell)
{
	t_lexer	*new;
	t_lexer *node;

	new = (t_lexer *)malloc(sizeof(t_lexer));
	if (!new)
	{
		exit(0); // to be changed
	}
	new->word = NULL;
	new->next = NULL;
	node = ft_lexer_last(shell->lexer);
	if (!node)
	{
		shell->lexer = new;
		new->prev = NULL;
	}
	else
	{
		(node)->next = new;
		new->prev = node;
	}
}
