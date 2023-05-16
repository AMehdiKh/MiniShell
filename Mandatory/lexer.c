/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hahadiou <hahadiou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/27 19:37:33 by ael-khel          #+#    #+#             */
/*   Updated: 2023/05/15 22:11:39 by hahadiou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_lexer(t_shell *shell)
{
	while (shell->list && !shell->exit_status)
	{
		ft_newnode(&(shell->lexer));
		if (*(shell->list->content) == '>')
			ft_write_redi(shell);
		else if (*(shell->list->content) == '<')
		 	ft_read_redi(shell);
		else if (*(shell->list->content) == '|')
		 	ft_lexer_pipe(shell);
		// else
		// 	ft_lexer_cmd(shell);
	}
	if (shell->lexer_status)
		ft_lexer_clear(&(shell->lexer));
	//free(shell->line);
	return (shell->lexer_status);
}

// void	ft_lexer_cmd(t_shell *shell)
// {
	
// }

void	ft_lexer_pipe(t_shell *shell)
{
	t_list	*tmp;
	char	*syntax_err;

	syntax_err = ft_syntax_err(shell->list, 1);
	if (syntax_err)
	{
		shell->lexer_status = 2;
		shell->exit_status = shell->lexer_status;
		ft_lstclear(&(shell->list));
		ft_dprintf(2, "minishell: syntax error near unexpected token `%s'\n",
			syntax_err);
		free(syntax_err);
		return ;
	}
	shell->lexer->type = PIPE;
	tmp = shell->list->next;
	ft_lstdelone(shell->list);
	shell->list = tmp;
}

void	ft_read_redi(t_shell *shell)
{
	t_list	*tmp;
	char	*syntax_err;

	syntax_err = ft_syntax_err(shell->list, 0);
	if (syntax_err)
	{
		shell->lexer_status = 2;
		shell->exit_status = shell->lexer_status;
		ft_lstclear(&(shell->list));
		ft_dprintf(2, "minishell: syntax error near unexpected token `%s'\n",
			syntax_err);
		free(syntax_err);
		return ;
	}
	if (ft_strlen(shell->list->content) == 1)
		shell->lexer->type = R_FILE;
	else
		shell->lexer->type = HEREDOC;
	shell->lexer->word = ft_remove_quotes(shell->list->next->content);
	tmp = shell->list->next->next;
	ft_lstdelone(shell->list->next);
	ft_lstdelone(shell->list);
	shell->list = tmp;
}

void	ft_write_redi(t_shell *shell)
{
	t_list	*tmp;
	char	*syntax_err;

	syntax_err = ft_syntax_err(shell->list, 0);
	if (syntax_err)
	{
		shell->lexer_status = 2;
		shell->exit_status = shell->lexer_status;
		ft_lstclear(&(shell->list));
		ft_dprintf(2, "minishell: syntax error near unexpected token `%s'\n",
			syntax_err);
		free(syntax_err);
		return ;
	}
	if (ft_strlen(shell->list->content) == 1)
		shell->lexer->type = W_T_FILE;
	else
		shell->lexer->type = W_A_FILE;
	shell->lexer->word = ft_remove_quotes(shell->list->next->content);
	tmp = shell->list->next->next;
	ft_lstdelone(shell->list->next);
	ft_lstdelone(shell->list);
	shell->list = tmp;
}

char	*ft_syntax_err(t_list *list, int pipe)
{
	if (ft_strlen(list->content) == 3 && !pipe)
		return (ft_substr(list->content, 0, 1));
	else if (ft_strlen(list->content) > 3 || (ft_strlen(list->content) > 1 && pipe))
		return (ft_substr(list->content, 0, 2));
	else if (!list->next)
		return (ft_strdup("newline"));
	else if (ft_strchr("<|>", *(list->next->content)))
	{
		if (ft_strlen(list->next->content) == 1)
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
		if (ft_strchr("(&);", tmp->content[0]) && tmp->content[0])
		{
			if (ft_strlen(shell->list->content) == 1 || ft_strchr("()", *(tmp->content)))
				syntax_err = ft_substr(tmp->content, 0, 1);
			else
				syntax_err = ft_substr(tmp->content, 0, 2);
			shell->exit_status = 2;
			ft_dprintf(2, "minishell: syntax error near unexpected token `%s'\n",
				syntax_err);
			free(syntax_err);
			free(shell->line);
			ft_lstclear(&(shell->list));
			return (2);
		}
		tmp = tmp->next;
	}
	return (0);
}
