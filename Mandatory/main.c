/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ael-khel <ael-khel@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/24 23:02:44 by ael-khel          #+#    #+#             */
/*   Updated: 2023/06/15 15:09:47 by ael-khel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_shell	*g_shell;

static void	destroy(t_shell	*shell, char *line)
{
	ft_clear(shell->env);
	free(shell);
	free(line);
}

static void	setup_shell(char **env)
{
	g_shell = (t_shell *)malloc(sizeof(t_shell));
	g_shell->env = ft_dup_env(env);
	config_signals();
}

static	char	*ft_unclosed_quote(char *line, size_t i, int quote)
{
	char	*read;

	while (line[i])
	{
		quote = 0;
		if (line[i] == 39 || line[i] == 34)
			quote = line[i];
		while ((quote && line[i++]))
		{
			if ((!line[i] && quote))
			{
				read = readline("> ");
				if (read == NULL)
					return (free(line), free(read), NULL);
				line = ft_strjoin(line, read, 4);
			}
			if (line[i] == quote)
				quote = 0;
		}
		++i;
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
		line = ft_unclosed_quote(line, 0, 0);
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
		ft_parser(g_shell->parser, g_shell, g_shell->lexer);
		ft_lexer_clear(&(g_shell->lexer));
	}
	return (0);
}
