/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ael-khel <ael-khel@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/24 23:02:44 by ael-khel          #+#    #+#             */
/*   Updated: 2023/05/04 02:10:31 by ael-khel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <stddef.h>
#include <stdint.h>
#include <unistd.h>


char	*ft_unclosed_quote(char *line)
{
	int		o_quote;
	int		c_quote;
	size_t	i;

	i = 0;
	while (line[i])
	{
		o_quote = 0;
		if (line[i] == 39 || line[i] == 34)
			o_quote = line[i];
		while (o_quote && line[i++] && !c_quote)
		{
			if (!line[i] && !c_quote)
			{
				write(1, "> ", 2);
				line = ft_strjoin(line, get_next_line(0), 4);
				// still have to handle ctl-D output
			}
			if (line[i] == o_quote)
				c_quote = 1;
		}
		c_quote = 0;
		++i;
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
		line = ft_unclosed_quote(line);
		shell->line = ft_strtrim(line, " ");
		free(line);
		if (shell->line && !*shell->line)
			continue ;
		add_history(shell->line);
		if (ft_lexer(shell))
			continue ;
	}
	return (0);
}
