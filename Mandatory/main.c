/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ael-khel <ael-khel@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/24 23:02:44 by ael-khel          #+#    #+#             */
/*   Updated: 2023/05/13 18:09:51 by ael-khel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <stddef.h>
#include <stdint.h>
#include <unistd.h>

int	ft_last_pipe(char *line)
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

char	*ft_unclosed_quote(char *line, size_t i, int pipe)
{
	int		quote;

	while (line[i] || pipe)
	{
		quote = 0;
		if (line[i] == 39 || line[i] == 34)
			quote = line[i];
		while ((quote && line[i++]) || pipe)
		{
			if ((!line[i] && quote) || pipe)
			{
				write(1, "> ", 2);
				line = ft_strjoin(line, get_next_line(0), 4);
			}
			if (line[i] == quote)
				quote = 0;
			pipe = 0;
		}
		++i;
		if (!line[i] && ft_last_pipe(line))
			line = ft_unclosed_quote(line, i, 1);
	}
	return (line);
}

//   export LDFLAGS="-L/Users/ael-khel/homebrew/opt/readline/lib"
//   export CPPFLAGS="-I/Users/ael-khel/homebrew/opt/readline/include/readline"
int	main(int ac, char **av, char **env)
{
	t_shell	shell[1];
	char	*line;

	ft_bzero(shell, sizeof(shell));
	shell->env = env;
	while (ac || av[0])
	{
		shell->lexer_status = 0;
		line = readline("MiniShell: $|");
		line = ft_unclosed_quote(line, 0, 0);
		shell->line = ft_strtrim(line, " ");
		free(line);
		printf("%s\n", shell->line);
		if (shell->line && !*shell->line)
			continue ;
		add_history(shell->line);
		if (ft_check_meta(shell))
			continue ;
		if (ft_lexer(shell))
			continue ;
		free(shell->line);
	}
	return (0);
}
