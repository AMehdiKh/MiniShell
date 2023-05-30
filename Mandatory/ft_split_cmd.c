/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split_cmd.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hahadiou <hahadiou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/02 03:49:14 by ael-khel          #+#    #+#             */
/*   Updated: 2023/05/30 04:04:35 by hahadiou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static size_t	ft_word_len(const char *s)
{
	int	quote;
	int	wl;

	wl = 0;
	while (!ft_isspace(s[wl]) && s[wl])
	{
		quote = 0;
		if (s[wl] == 39 || s[wl] == 34)
			quote = s[wl++];
		while (quote && s[wl] != quote)
			++wl;
		++wl;
	}
	return (wl);
}

static char	**ft_insert(const char *s, size_t wc)
{
	char	**ptr;
	size_t	wl;
	size_t	x;

	ptr = ft_calloc(wc + 1, sizeof(char *));
	if (!ptr)
		return (NULL);
	x = 0;
	while (*s && x < wc)
	{
		while (ft_isspace(*s))
			++s;
		wl = ft_word_len(s);
		ptr[x] = ft_remove_quotes(ft_substr(s, 0, wl), 1);
		if (!ptr[x])
			return (ft_clear_split(ptr, x));
		s += wl;
		++x;
	}
	return (ptr);
}

static size_t	ft_word_count(const char *s)
{
	int		quote;
	size_t	wc;

	wc = 0;
	while (*s)
	{
		while (ft_isspace(*s))
			++s;
		if (*s)
			++wc;
		while (!ft_isspace(*s) && *s)
		{
			quote = 0;
			if (*s == 39 || *s == 34)
				quote = *s++;
			while (quote && *s != quote)
				++s;
			++s;
		}
	}
	return (wc);
}

char	**ft_split_cmd(char const *s)
{
	if (!s || !*s)
		return (NULL);
	return (ft_insert(s, ft_word_count(s)));
}
