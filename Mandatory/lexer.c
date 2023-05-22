/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hahadiou <hahadiou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/27 19:37:33 by ael-khel          #+#    #+#             */
/*   Updated: 2023/05/22 18:36:24 by hahadiou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_lexer(t_shell *shell)
{
	while (shell->list && !shell->exit_status)
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

char	*ft_arg_join(t_shell *shell)
{
	t_list	*tmp;
	char	*cmd;

	cmd = ft_strdup("");
	while (shell->list && !ft_strchr("<|>", *(shell->list->content)))
	{
		tmp = shell->list;
		cmd = ft_strjoin(cmd, ft_strjoin(ft_remove_quotes(expander(shell->list->content, shell), 1), " ", 1), 4);
		shell->list = shell->list->next;
		ft_lstdelone(tmp);
	}
	return (cmd);
}

int	ft_builtin_check(char *s, t_shell *shell)
{
	char	*tmp;
	tmp = ft_remove_quotes(expander(s, shell), 1);
	if (!ft_strncmp(tmp, "exit", ft_strlen("exit") + 1)
		|| !ft_strncmp(tmp, "echo", ft_strlen("echo") + 1)
		|| !ft_strncmp(tmp, "cd", ft_strlen("cd") + 1)
		|| !ft_strncmp(tmp, "pwd", ft_strlen("pwd") + 1)
		|| !ft_strncmp(tmp, "export", ft_strlen("export") + 1)
		|| !ft_strncmp(tmp, "unset", ft_strlen("unset") + 1)
		|| !ft_strncmp(tmp, "env", ft_strlen("env") + 1))
	{
		free(tmp);
		return (1);
	}
	free(tmp);
	return (0);
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

// void	setup_red(t_shell *shell, char *filename, t_token type, bool expand)
// {
// 	int	fd;
	
// 	if (type == W_T_FILE)
// 	{
// 		fd = open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
// 		if (fd < 0)
// 		{
// 			ft_dprintf(2, "Erooooor while opening the file\n"); // to check the type of error
// 		}
// 	}
// 	else if (type == W_A_FILE)
// 	{
// 		fd = open(filename, O_WRONLY | O_CREAT | O_APPEND, 0644);
// 		if (fd < 0)
// 		{
// 			ft_dprintf(2, "Erooooor while opening the file\n"); // to check the type of error
// 		}
// 	}
// 	else if (type == R_FILE)
// 	{
// 		fd = open(filename, O_RDONLY);
// 		if (fd < 0)
// 		{
// 			ft_dprintf(2, "minishell: %s: No such file or directory\n", filename);
// 		}
// 	}
// 	else if (type == HEREDOC)
// 	{
// 		heredoc(shell, filename, expand);
// 	}
// }

// int	search_quotes(const char* str)
// {
//     while (*str)
// 	{
//         if (*str == 39 || *str == 34)
// 		{
//             return (1);
//         }
//         str++;
//     }
//     return (0);
// }

void	ft_read_redi(t_shell *shell, t_lexer *node)
{
	t_list	*tmp;
	char	*syntax_err;
	// bool	expand;
	
	// expand = true;
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
	// if (search_quotes(shell->list->next->content))
	// 	expand = false;
	node->word = ft_remove_quotes(shell->list->next->content, 0);
	// setup_red(shell, node->word, node->type, expand);
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
	// setup_red(shell, node->word, node->type, true);
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
