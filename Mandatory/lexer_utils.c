/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hahadiou <hahadiou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/14 23:08:45 by hahadiou          #+#    #+#             */
/*   Updated: 2023/05/30 03:55:42 by hahadiou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "minishell.h"

char	*ft_syntax_err(t_list *list, int pipe)
{
	if (ft_strlen(list->content) == 3 && !pipe)
		return (ft_substr(list->content, 0, 1));
	else if (ft_strlen(list->content) > 3 || (ft_strlen(list->content) > 1 && pipe))
		return (ft_substr(list->content, 0, 2));
	else if (!list->next)
		return (ft_strdup("newline"));
	else if (ft_strchr("<|>", *(list->next->content)) && !pipe)
	{
		if (ft_strlen(list->next->content) == 1)
			return (ft_substr(list->next->content, 0, 1));
		else
			return (ft_substr(list->next->content, 0, 2));
	}
	else if (ft_strchr("|", *(list->next->content)) && pipe)
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

char	*ft_arg_join(t_shell *shell)
{
	t_list	*tmp;
	char	*cmd;

	cmd = ft_strdup("");
	while (shell->list && !ft_strchr("<|>", *(shell->list->content)))
	{
		tmp = shell->list;
		cmd = ft_strjoin(cmd, ft_strjoin(expander(shell->list->content, shell), " ", 1), 4);
		shell->list = shell->list->next;
		ft_lstdelone(tmp);
	}	
	return (cmd);
}
