/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ael-khel <ael-khel@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/15 11:03:05 by ael-khel          #+#    #+#             */
/*   Updated: 2022/10/15 11:03:05 by ael-khel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <stddef.h>

static int	ft_end_quoted(const char **s, int *quote, int option)
{
	int	i;

	i = 0;
	if (**s == 39 || **s == 34)
	{
		*quote = **s;
		*s += 1;
		while (*(*s + i))
		{
			if (*(*s + i) == *quote && (*(*s + (i + 1)) == ' '
					|| *(*s + (i + 1)) == '\0'))
				return (1 + option);
			else if (*(*s + i) == ' ' && *(*s + (i - 1)) != *quote
				&& *(*s + (i + 1)) == *quote
				&& (*(*s + (i + 2)) != '\0' && *(*s + (i + 2)) != ' '))
				return (0);
			++i;
		}
	}
	else
		*quote = 0;
	return (0);
}

static size_t	ft_word_len(const char **s, char c, int option, int *e_quote)
{
	size_t	wl;
	int		quote;

	wl = 0;
	*e_quote = ft_end_quoted(s, &quote, option);
	*s -= ((quote && !*e_quote) || *e_quote == 2);
	wl += ((quote && !*e_quote) || *e_quote == 2);
	while (*(*s + wl))
	{
		if ((*e_quote && *(*s + wl) == quote && (*(*s + (wl + 1)) == ' '
					|| *(*s + (wl + 1)) == '\0'))
			|| (!*e_quote && *(*s + wl) == c))
		{
			wl += (*e_quote == 2);
			break ;
		}
		++wl;
	}
	return (wl);
}

static char	**ft_insert(const char *s, char c, size_t wc, int option)
{
	char	**ptr;
	int		e_quote;
	size_t	wl;
	size_t	x;

	ptr = ft_calloc(wc + 1, sizeof(char *));
	if (!ptr)
		return (NULL);
	x = 0;
	while (*s && x < wc)
	{
		wl = 0;
		while (*s == c && *s)
			++s;
		wl = ft_word_len(&s, c, option, &e_quote);
		ptr[x] = malloc(wl + 1);
		if (!ptr[x])
			return (ft_clear_split(ptr, x));
		ft_strlcpy(ptr[x], s, wl + 1);

		s += (wl + (e_quote == 1));
		++x;
	}
	return (ptr);
}

static size_t	ft_word_count(const char *s, char c)
{
	int		e_quote;
	int		quote;
	size_t	wc;

	wc = 0;
	while (*s)
	{
		while (*s == c && *s)
			++s;
		if (*s)
			++wc;
		e_quote = ft_end_quoted(&s, &quote, 0);
		while (*s)
		{
			if ((e_quote && *s == quote && (*(s + 1) == ' '
						|| *(s + 1) == '\0')) || (!e_quote && *s == c))
			{
				s += e_quote;
				break ;
			}
			++s;
		}
	}
	return (wc);
}

char	**ft_split(char const *s, char c, int option)
{
	if (!s || !*s)
		return (NULL);
	return (ft_insert(s, c, ft_word_count(s, c), option));
}
