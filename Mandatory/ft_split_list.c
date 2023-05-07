/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split_list.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ael-khel <ael-khel@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/03 15:53:59 by ael-khel          #+#    #+#             */
/*   Updated: 2023/05/04 16:56:08 by ael-khel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <stddef.h>
#include <stdint.h>
#include <unistd.h>

t_list	*ft_split_list(const char *s, char c, int option)
{
	t_list	*head;
	int		token;
	int		i;

	head = NULL;
	while (*s)
	{
		while (*s && *s == c)
			++s;
		i = 0;
		if (ft_strchr("(<|>)&", *s) && *s)
		{
			token = s[i++];
			while (s[i] == token)
				++i;
		}
		else
			i = ft_not_token(s);(
		if (!option)
			ft_lstadd_back(&head, ft_lstnew(ft_substr(s, 0, i)));
		else
			ft_lstadd_back(&head, ft_lstnew(ft_remove_quotes(ft_substr(s, 0, i))));
		s += i;
	}
	return (head);
}

char	*ft_remove_quotes(char *s)
{
	char	*str;
	int		quote;
	int		i;
	int		j;

	if (ft_strchr("(<|>)&", *s))
		return (s);
	str = ft_calloc((ft_strlen(s) - ft_quotes_number(s)) + 1, sizeof(char));
	if (!str)
		return (NULL);
	i = 0;
	j = 0;
	while (s[i])
	{
		quote = 0;
		if (s[i] == 39 || s[i] == 34)
			quote = s[i++];
		while (quote && s[i] != quote)
			str[j++] = s[i++];
		if (quote)
			++i;
		str[j++] = s[i++];
	}
	free(s);
	return (str);
}

int	ft_quotes_number(char *s)
{
	int	n_quote;
	int	quote;

	n_quote = 0;
	while (*s)
	{
		quote = 0;
		if (*s == 39 || *s == 34)
		{
			quote = *s++;
			n_quote += 2;
		}
		while (quote && *s != quote)
			++s;
		++s;
	}
	return (n_quote);
}

int	ft_not_token(char *s)
{
	int	quote;
	int	i;

	i = 0;
	while (!ft_strchr("(<|>)& ", s[i]) && s[i])
	{
		quote = 0;
		if (s[i] == 39 || s[i] == 34)
			quote = s[i++];
		while (quote && s[i] != quote)
			++i;
		++i;
	}
	return (i);
}
