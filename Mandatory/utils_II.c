/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_II.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hahadiou <hahadiou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/09 19:32:45 by hahadiou          #+#    #+#             */
/*   Updated: 2023/05/30 18:53:38 by hahadiou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static bool	ft_search(char *s, char c)
{
	int	i;

	i = 0;
	while (s[i])
	{
		if (s[i] == c)
			return (true);
		i++;
	}
	return (false);
}

int	ft_heredoc(t_shell *shell, t_lexer *lexer, int stdin)
{
	char	*heredoc[2];
	int		pfds[2];
	bool	expand;

	pipe(pfds);
	expand = true;
	if (ft_search(lexer->word, '\'') || ft_search(lexer->word, '\"'))
	{
		expand = false;
		lexer->word = ft_remove_quotes(lexer->word, 0);
	}
	while (1)
	{
		ft_dprintf(2, "> ");
		heredoc[0] = get_next_line(stdin);
		if (ft_strnstr(heredoc[0], lexer->word, ft_strlen(lexer->word)))
			break ;
		heredoc[1] = ft_expander(heredoc[0], shell);
		if (heredoc[1] && expand)
			ft_dprintf(pfds[1], "%s", heredoc[1]);
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
	t_lexer	*node;

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
