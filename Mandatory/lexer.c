/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ael-khel <ael-khel@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/27 19:37:33 by ael-khel          #+#    #+#             */
/*   Updated: 2023/05/04 18:02:01 by ael-khel         ###   ########.fr       */
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
	}
	return (shell->lexer_status);
}

void	ft_write_redi(t_shell *shell, t_list **list)
{
	shell->lexer_status = ft_w_redi_error(*list);
	shell->exit_status = shell->lexer_status;
	if (shell->lexer_status)
	{
		ft_lstclear(lst, free);
		return ;
	}
	if (ft_strlen(*list->content) == 1)
		shell->lexer->type = W_T_FILE;
	else
		shell->lexer->type = W_A_FILE;
	shell->lexer->word = ft_remove_quotes(*list->next->content);
	ft
}

void	ft_w_redi_error(t_list *list)
{
	if (ft_strlen(list->content) == 3)
		ft_dprintf(2, "MiniShell: syntax error near unexpected token `>'\n");
	else if (ft_strlen(list->content) > 3)
		ft_dprintf(2, "MiniShell: syntax error near unexpected token `>>'\n");
	else if (!list->next)
		ft_dprintf(2,
			"MiniShell: syntax error near unexpected token `newline'\n");
	else if (ft_strchr("()", *(list->next->content)))
		ft_dprintf(2, "MiniShell: syntax error near unexpected token `%c'\n",
			*(*list->next->content));
	else if (ft_strchr("<|>&", *(list->next->content)))
	{
		if (ft_strlen(*list->next->content) == 1)
			ft_dprintf(2, "MiniShell: syntax error near unexpected token `%c'\n",
				*(list->next->content));
		else
		{
			ft_dprintf(2, "MiniShell: syntax error near unexpected token `");
			write(2, list->next->content, 2);
			ft_dprintf(2, "'\n");
		}
	}
	else
		return (0);
	return (2);
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


int	valid_filename(t_shell *shell, char **filename)
{
	filename = ft_split(shell->line + shell->i, ' ', 1);
	if (!filename)
		ft_dprintf(2,
			"MiniShell: syntax error near unexpected token `newline'");
	else if (ft_strnstr(filename[0], "||", 2))
		ft_dprintf(2, "MiniShell: syntax error near unexpected token `||'");
	else if (ft_strnstr(filename[0], "|", 1))
		ft_dprintf(2, "MiniShell: syntax error near unexpected token `|'");
	else if (ft_strnstr(filename[0], "&&", 2))
		ft_dprintf(2, "MiniShell: syntax error near unexpected token `&&'");
	else if (ft_strnstr(filename[0], "&", 1))
		ft_dprintf(2, "MiniShell: syntax error near unexpected token `&'");
	else if (ft_strnstr(filename[0], "(", 1))
		ft_dprintf(2, "MiniShell: syntax error near unexpected token `('");
	else if (ft_strnstr(filename[0], ")", 1))
		ft_dprintf(2, "MiniShell: syntax error near unexpected token `)'");
	else
	{
		shell->lexer->word = ft_strdup(filename[0]);
		ft_clear(filename);
		return (0);
	}
	ft_clear(filename);
	return (2);
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
