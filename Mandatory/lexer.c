/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hahadiou <hahadiou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/27 19:37:33 by ael-khel          #+#    #+#             */
/*   Updated: 2023/05/30 03:59:05 by hahadiou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_lexer(t_shell *shell)
{
	while (shell->list && !shell->lexer_status)
	{
		ft_newnode(shell);
		if (*(shell->list->content) == '>')
			ft_write_redi(shell, ft_lexer_last(shell->lexer));
		else if (*(shell->list->content) == '<')
		 	ft_read_redi(shell, ft_lexer_last(shell->lexer));
		else if (*(shell->list->content) == '|')
		 	ft_lexer_pipe(shell, ft_lexer_last(shell->lexer));
		else
			ft_lexer_cmd(shell, ft_lexer_last(shell->lexer));
	}
	if (shell->lexer_status)
		ft_lexer_clear(&(shell->lexer));
	free(shell->line);
	return (shell->lexer_status);
}

void	ft_lexer_cmd(t_shell *shell, t_lexer *node)
{
	if (ft_builtin_check(shell->list->content, shell))
		node->type = BUILTIN;
	else
		node->type = CMD;
	node->word = ft_arg_join(shell);
}

void	ft_lexer_pipe(t_shell *shell, t_lexer *node)
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
	node->type = PIPE;
	tmp = shell->list->next;
	ft_lstdelone(shell->list);
	shell->list = tmp;
}

void	ft_read_redi(t_shell *shell, t_lexer *node)
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
		node->type = R_FILE;
	else
		node->type = HEREDOC;
	node->word = ft_remove_quotes(shell->list->next->content, 0);
	tmp = shell->list->next->next;
	ft_lstdelone(shell->list->next);
	ft_lstdelone(shell->list);
	shell->list = tmp;
}

void	ft_write_redi(t_shell *shell, t_lexer *node)
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
		node->type = W_T_FILE;
	else
		node->type = W_A_FILE;
	node->word = ft_remove_quotes(shell->list->next->content, 0);
	tmp = shell->list->next->next;
	ft_lstdelone(shell->list->next);
	ft_lstdelone(shell->list);
	shell->list = tmp;
}
