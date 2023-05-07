/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ael-khel <ael-khel@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/27 19:37:33 by ael-khel          #+#    #+#             */
/*   Updated: 2023/05/07 23:26:39 by ael-khel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lexer.h"
#include "minishell.h"

int	ft_lexer(t_shell *shell)
{
	t_list	*list;

	list = shell->list;
	while (list && !shell->exit_status)
	{
		ft_newnode(shell, &shell->lexer);
		if (*list->content == '>')
			ft_write_redi(shell, &list);
		else if (*list->content == '<')
			ft_read_redi(shell, &list);
	}
	ft_lstclear(shell->lexer);
	return (shell->lexer_status);
}

void	ft_read_redi(t_shell *shell, t_list **list)
{
	t_list	*tmp;
	char	*syntax_err;

	syntax_err = ft_syntax_err(*list);
	if (syntax_err)
	{
		shell->lexer_status = 2;
		shell->exit_status = shell->lexer_status;
		ft_lstclear(lst);
		ft_dprintf(2, "MiniShell: syntax error near unexpected token `%s'\n",
			syntax_err);
		free(syntax_err);
		return ;
	}
	if (ft_strlen(*list->content) == 1)
		shell->lexer->type = R_FILE;
	else
		shell->lexer->type = HEREDOC;
	shell->lexer->word = ft_remove_quotes(*list->next->content);
	tmp = *list->next->next;
	ft_lstdelone(*list->next);
	ft_lstdelone(*list);
	*list = tmp;
}

void	ft_write_redi(t_shell *shell, t_list **list)
{
	t_list	*tmp;
	char	*syntax_err;

	syntax_err = ft_syntax_err(list);
	if (syntax_err)
	{
		shell->lexer_status = 2;
		shell->exit_status = shell->lexer_status;
		ft_lstclear(lst);
		ft_dprintf(2, "MiniShell: syntax error near unexpected token `%s'\n",
			syntax_err);
		free(syntax_err);
		return ;
	}
	if (ft_strlen(*list->content) == 1)
		shell->lexer->type = W_T_FILE;
	else
		shell->lexer->type = W_A_FILE;
	shell->lexer->word = ft_remove_quotes(*list->next->content);
	tmp = *list->next->next;
	ft_lstdelone(*list->next);
	ft_lstdelone(*list);
	*list = tmp;
}

char	*ft_syntax_err(t_list *list)
{
	if (ft_strlen(list->content) == 3)
		return (ft_substr(list->content, 0, 1));
	else if (ft_strlen(list->content) > 3)
		return (ft_substr(list->content, 0, 2));
	else if (!list->next)
		return (ft_strdup("newline"));
	else if (ft_strchr("<|>", *(list->next->content)))
	{
		if (ft_strlen(*list->next->content) == 1)
			return (ft_substr(list->next->content, 0, 1));
		else
			return (ft_substr(list->next->content, 0, 2));
	}
	return (NULL);
}

int	ft_check_meta(t_shell *shell)
{
	t_list	*tmp;
	char	*syntax_err;

	shell->list = ft_split_list(shell->line);
	tmp = shell->list;
	while (tmp)
	{
		if (ft_strchr("(&);", *tmp->content))
		{
			if (ft_strlen(list->content) == 1
				|| ft_strchr("()", *tmp->content))
				syntax_err = ft_substr(tmp->content, 0, 1);
			else
				syntax_err = ft_substr(tmp->content, 0, 2);
			shell->exit_status = 2;
			ft_dprintf(2, "MiniShell: syntax error near unexpected token `%s'\n",
				syntax_err);
			free(syntax_err);
			free(shell->line);
			ft_lstclear(shell->list);
			return (2);
		}
		tmp = tmp->next;
	}
	return (0);
}

void	ft_readRedi_heredoc(t_shell *shell)
{
	char	**filename;

	shell->lexer->type = R_FILE;
	shell->i += 1;
	if (!ft_strncmp((shell->line + shell->i), "<<", 1))
	{
		shell->lexer->type = HEREDOC;
		shell->i += 1;
	}
	while (shell->line[shell->i] == ' ')
		++shell->i;
	shell->lexer_status = valid_filename(shell, NULL);
	shell->exit_status = shell->lexer_status;
	if (shell->lexer_status)
		return ;
	if (ft_strncmp((shell->line + shell->i), shell->lexer->word,
			ft_strlen(shell->lexer->word)))
		shell->i += (ft_strlen(filename[0]) + 2);
	else
		shell->i += ft_strlen(filename[0]);
}

void	ft_write_redi(t_shell *shell)
{
	char	**filename;

	if (ft_strlen)
	shell->lexer->type = W_T_FILE;
	shell->i += 1;
	if (!ft_strncmp((shell->line + shell->i), ">", 1))
	{
		shell->lexer->type = W_A_FILE;
		shell->i += 1;
	}
	while (shell->line[shell->i] == ' ')
		++shell->i;
	shell->lexer_status = valid_filename(shell, NULL);
	shell->exit_status = shell->lexer_status;
	if (shell->lexer_status)
		return ;
	if (ft_strncmp((shell->line + shell->i), shell->lexer->word,
			ft_strlen(shell->lexer->word)))
		shell->i += (ft_strlen(filename[0]) + 2);
	else
		shell->i += ft_strlen(filename[0]);
	while (shell->line[shell->i] == ' ')
		++shell->i;
}

void	ft_newnode(t_shell *shell, t_lexer **node)
{
	t_lexer	*new;

	new = malloc(sizeof(t_lexer));
	if (!new)
	{
		exit(0);
	}
	if (!*node)
	{
		*node = new;
		new->prev = NULL;
	}
	else
	{
		(*node)->next = new;
		new->prev = *node;
	}
		new->next = NULL;
}
