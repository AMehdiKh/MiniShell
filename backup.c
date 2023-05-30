void	ft_lexer_cmd(t_shell *shell)
{
	if (ft_builtin_check(shell->list->content))
		shell->lexer->type = BUILTIN;
	else
		shell->lexer->type = CMD;
	shell->lexer->word = ft_arg_join(shell);
}

int	ft_builtin_check(char *s)
{
	char	*tmp;

	tmp = ft_remove_quotes(ft_expander(s));
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
		cmd = ft_strjoin(cmd, ft_strjoin(ft_remove_quotes(ft_expander(shell->list->content)), " ", 1), 4);
		shell->list = shell->list->next;
		ft_lstdelone(tmp);
	}
	return (cmd);
}


int	ft_check_dollar(char *s)
{
	int		quote;

	while (*s)
	{
		quote = 0;
		if (*s == 39)
			quote = *s++;
		while (quote == 39 && *s != quote)
			++s;
		if (*s == '$')
			return (1);
		++s;
	}
	return (0);
}

char	*ft_dollar_expansion(t_shell *shell, char *s, size_t *i)
{
	char	*var_name;
	size_t	start;

	*i += 1;
	if (s[*i] == '?' && (*i)++)
		return (ft_itoa(shell->exit_status));
	if (ft_isdigit(s[*i]) && (*i)++)
		return (ft_strdup(s + *i));
	if ((!ft_isalpha(s[*i]) && s[*i] != '_'))
		return (ft_strdup(s + (*i - 1)));
	start = *i;
	while ((ft_isalnum(s[*i]) || s[*i] == '_') && !ft_isspace(s[*i]) && s[*i])
		(*i)++;
	var_name = ft_strjoin(ft_substr(s, start, *i - 1), "=", 1);
	start = 0;
	while (shell->env[start])
	{
		if (!ft_strncmp(var_name, shell->env[start], ft_strlen(var_name)))
		{
			free(var_name);
			return (ft_strdup(shell->env[start]));
		}
		++start;
	}
	free(var_name);
	return (ft_strdup(""));
}

char	*ft_expander(t_shell *shell, char *s)
{
	char	*str;
	int		quote;
	size_t	i;
	size_t	j;

	if (!ft_check_dollar(s))
		return (s);
	str = ft_strdup("");
	i = 0;
	j = 0;
	while (s[i])
	{
		quote = 0;
		if (s[i] == 39)
			quote = s[i++];
		while (quote && s[i++] != quote)
			;
		if (s[i] == '$')
		{
			str = ft_strjoin(str, ft_substr(s, j, i), 4);
			str = ft_strjoin(str, ft_dollar_expansion(shell, s, &i), 4);
			j = i;
		}
		else if (s[i])
			++i;
	}
	str = ft_strjoin(str, ft_substr(s, j, i), 4);
	return (str);
}



void	ft_lexer_pipe(t_shell *shell)
{
	t_list	*tmp;
	char	*syntax_err;

	syntax_err = ft_syntax_err(shell->list, 1);
	if (syntax_err)
	{
		shell->lexer_status = 2;
		shell->exit_status = shell->lexer_status;
		ft_lstclear(shell->list);
		ft_dprintf(2, "MiniShell: syntax error near unexpected token `%s'\n",
			syntax_err);
		free(syntax_err);
		return ;
	}
	shell->lexer->type = PIPE;
	tmp = shell->list->next;
	ft_lstdelone(shell->list);
	shell->list = tmp;
}
