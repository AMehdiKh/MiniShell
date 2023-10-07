/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ael-khel <ael-khel@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/04 15:31:11 by hahadiou          #+#    #+#             */
/*   Updated: 2023/06/15 15:06:05 by ael-khel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_check_pipe(t_lexer *lexer)
{
	while (lexer)
	{
		if (lexer->type == PIPE)
			return (1);
		lexer = lexer->next;
	}
	return (0);
}

int	ft_check_builtin(t_lexer *lexer)
{
	while (lexer)
	{
		if (lexer->type == BUILTIN)
			return (1);
		lexer = lexer->next;
	}
	return (0);
}

void	ft_save_std(t_parser *parser)
{
	parser->_stdin = dup(0);
	parser->_stdout = dup(1);
}

int	exec_builtin_child(t_lexer *lexer, t_shell *shell)
{
	char	**argv;

	argv = ft_split_cmd(lexer->word);
	if (!ft_strncmp(argv[0], "echo", 5))
		return (ft_echo_builtin(ft_count_strings(argv), argv));
	else if (!ft_strncmp(argv[0], "pwd", 4))
		return (ft_pwd_builtin());
	else if (!ft_strncmp(argv[0], "env", 4))
		return (ft_env_builtin(argv, shell));
	else if (!ft_strncmp(argv[0], "cd", 3))
		return (ft_cd_builtin(argv[1], shell));
	else if (!ft_strncmp(argv[0], "unset", 6))
		return (ft_unset_builtin((argv[1]), shell->env));
	else if (!ft_strncmp(argv[0], "exit", 5))
		ft_exit_builtin(ft_count_strings(argv), argv[1], shell);
	else if (!ft_strncmp(argv[0], "export", 7))
		return (ft_export_builtin(shell, argv));
	return (0);
}

int	ft_heredoc_cmp(char *limiter, char *heredoc)
{
	size_t	i;

	if (!*limiter && *heredoc == '\n')
		return (1);
	if (ft_strlen(limiter) != ft_strlen(heredoc) - 1)
		return (0);
	i = 0;
	while (limiter[i])
	{
		if (limiter[i] != heredoc[i])
			return (0);
		++i;
	}
	return (1);
}
