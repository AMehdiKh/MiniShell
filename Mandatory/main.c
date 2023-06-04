/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hahadiou <hahadiou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/24 23:02:44 by ael-khel          #+#    #+#             */
/*   Updated: 2023/06/04 15:26:45 by hahadiou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_shell	*g_shell;

static void	destroy(t_shell	*shell, char *line)
{
	ft_clear(shell->env);
	free(shell);
	free(line);
	ft_dprintf(STDOUT_FILENO, "exit\n");
}

static void	setup_shell(char **env)
{
	g_shell = (t_shell *)malloc(sizeof(t_shell));
	g_shell->env = ft_dup_env(env);
	config_signals();
}

static int	ft_last_pipe(char *line)
{
	size_t	i;
	size_t	j;

	i = 0;
	while (ft_isspace(line[i]) || line[i] == '|')
		++i;
	if (!line[i])
		return (0);
	i = ft_strlen(line) - 1;
	while (ft_isspace(line[i]) && i != 0)
		--i;
	j = 0;
	while (i != 0 && line[i] == '|')
	{
		++j;
		--i;
	}
	if (j <= 2 && j)
		return (1);
	return (0);
}

static	char	*ft_unclosed_quote(char *line, size_t i, int pipe, int quote)
{
	char	*read;

	while (line[i] || pipe)
	{
		quote = 0;
		if (line[i] == 39 || line[i] == 34)
			quote = line[i];
		while ((quote && line[i++]) || pipe)
		{
			if ((!line[i] && quote) || pipe)
			{
				read = readline("> ");
				if (read == NULL)
					return (free(line), free(read), NULL);
				line = ft_strjoin(line, read, 4);
			}
			if (line[i] == quote)
				quote = 0;
			pipe = 0;
		}
		++i;
		if (!line[i] && ft_last_pipe(line))
			line = ft_unclosed_quote(line, i, 1, quote);
	}
	return (line);
}

int	main(int ac, char **av, char **env)
{
	char		*line;

	setup_shell(env);
	while (ac || av[1])
	{
		g_shell->lexer_status = 0;
		line = readline("⥴ ");
		if (line == NULL)
			return (destroy(g_shell, line), 0);
		line = ft_unclosed_quote(line, 0, 0, (int){0});
		if (line == NULL)
			continue ;
		g_shell->line = ft_strtrim(line, " \t");
		add_history(g_shell->line);
		free(line);
		if (g_shell->line && !*g_shell->line)
			continue ;
		if (ft_check_meta(g_shell))
			continue ;
		if (ft_lexer(g_shell))
			continue ;
		ft_parser((t_parser [1]){0}, g_shell, g_shell->lexer);
		ft_lexer_clear(&(g_shell->lexer));
	}
	return (0);
}
