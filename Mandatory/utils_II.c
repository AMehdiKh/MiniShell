/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_II.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ael-khel <ael-khel@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/09 19:32:45 by hahadiou          #+#    #+#             */
/*   Updated: 2023/06/15 15:02:53 by ael-khel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <malloc/_malloc.h>

static	char	*check_quotes(char *word, bool *expand)
{
	if (ft_search(word, '\'') || ft_search(word, '\"'))
	{
		*expand = false;
		return (word = ft_remove_quotes(word, 0));
	}
	else
		*expand = true;
	return (word);
}

int	ft_heredoc(t_shell *shell, t_lexer *lexer)
{
	char	*heredoc[2];
	int		pfds[2];
	bool	expand;

	pipe(pfds);
	lexer->word = check_quotes(lexer->word, &expand);
	while (1)
	{
		ft_dprintf(2, "> ");
		heredoc[0] = get_next_line(shell->parser->_stdin);
		if (ft_heredoc_cmp(lexer->word, heredoc[0]))
			break ;
		heredoc[1] = ft_expander(heredoc[0], shell);
		if (heredoc[1] && expand)
		{
			ft_dprintf(pfds[1], "%s", heredoc[1]);
			free(heredoc[1]);
		}
		else
			ft_dprintf(pfds[1], "%s", heredoc[0]);
		free(heredoc[0]);
	}
	free(heredoc[0]);
	close(pfds[1]);
	return (pfds[0]);
}

int	ft_file2(t_lexer *lexer)
{
	int	fd;

	if (lexer->type == W_A_FILE)
		fd = ft_open(lexer->word, O_CREAT | O_APPEND | O_WRONLY, 0644);
	if (lexer->type == W_T_FILE)
		fd = ft_open(lexer->word, O_CREAT | O_TRUNC | O_WRONLY, 0644);
	return (fd);
}

void	ft_close_pipe(t_parser *parser)
{
	if (parser->prev_in)
		close(parser->prev_in);
	parser->prev_in = dup(parser->pipefd[0]);
	close(parser->pipefd[0]);
	close(parser->pipefd[1]);
}

void	ft_newnode(t_shell *shell)
{
	t_lexer	*new;
	t_lexer	*node;

	new = (t_lexer *)malloc(sizeof(t_lexer));
	if (!new)
		return ;
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
