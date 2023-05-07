/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ael-khel <ael-khel@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/29 21:52:08 by ael-khel          #+#    #+#             */
/*   Updated: 2023/05/04 00:24:19 by ael-khel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/wait.h>
#include "LibFT/include/libft.h"

size_t	ft_word_count(const char *s, char c)
{
	int		quote;
	int		token;
	int		i;

	while (*s)
	{
		while (*s && *s == c)
			++s;
		i = 0;
		while (s[i] && s[i] != c)
		{
			quote = 0;
			if (ft_strchr("(<|>)&", s[i]) && s[i])
			{
				token = s[i++];
				while (s[i] == token)
					++i;
				break ;
			}
			if (s[i] == 39 || s[i] == 34)
				quote = s[i++];
			while (s[i] != quote)
				++i;
			if (ft_strchr("(<|>)&", s[i]) && s[i])
				break ;
			++i;
		}
		write(1, s, i + 1);
		write(1, "\n", 1);
		s += i;
	}
	return (0);
}
int	main(int ac, char **av, char **env)
{
	
	
	return (0);
}
