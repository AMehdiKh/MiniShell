/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split_shell.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ael-khel <ael-khel@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/02 03:49:14 by ael-khel          #+#    #+#             */
/*   Updated: 2023/05/02 06:20:33 by ael-khel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <stddef.h>
#include <stdint.h>
#include <unistd.h>

static char	**ft_insert(const char *s, char c, size_t wc, int option)
{
	char	**ptr;
	size_t	wl;
	size_t	ws;
	size_t	x;

	ptr = ft_calloc(wc + 1, sizeof(char *));
	if (!ptr)
		return (NULL);
	x = 0;
	while (*s && x < wc)
	{
		while (*s && *s == c)
			++s;
		wl = ft_word_len(s, c, &ws, option);
		ptr[x] = malloc(wl + 1);
		if (!ptr[x])
			return (ft_clear_split(ptr, x));
		ft_strlcpy(ptr[x], s, wl + 1);
		s += ws;
		++x;
	}
	return (ptr);
}

static size_t	ft_word_count(const char *s, char c)
{
	int		quote;
	size_t	wc;

	wc = 0;
	while (*s)
	{
		while (*s && *s == c)
			++s;
		if (*s)
			++wc;
		while (*s && *s != c)
		{
			quote = 0;
			if (*s == 39 || *s == 34)
				quote = *s++;
			while (quote && *s && *s != quote)
				++s;
			++s;
		}
	}
	return (wc);
}

static size_t	ft_word_len(const char *s, char c, size_t *ws, int option)
{
	int		n_quote;
	int		quote;
	size_t	wl;

	n_quote = 0;
	wl = 0;
	while (s[wl] && s[wl] != c)
	{
		quote = 0;
		if (s[wl] == 39 || s[wl] == 34)
			quote = s[wl++];
		if (quote)
			++n_quote;
		while (quote && s[wl] && s[wl] != quote)
			++wl;
		++wl;
	}
	*ws = wl;
	if (!option)
		wl -= (n_quote * 2);
	return (wl);
}

char	**ft_split(char const *s, char c, int option)
{
	if (!s || !*s)
		return (NULL);
	return (ft_insert(s, c, ft_word_count(s, c), option));
}
