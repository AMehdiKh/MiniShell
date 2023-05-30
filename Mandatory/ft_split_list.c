/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split_list.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hahadiou <hahadiou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/03 15:53:59 by ael-khel          #+#    #+#             */
/*   Updated: 2023/05/30 16:30:12 by hahadiou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_list	*ft_split_list(const char *s)
{
	t_list	*head;
	int		token;
	int		i;

	head = NULL;
	while (*s)
	{
		while (*s && ft_isspace(*s))
			++s;
		i = 0;
		if (ft_strchr("(<|>)&;", *s) && *s)
		{
			token = s[i++];
			while (s[i] == token)
				++i;
		}
		else
			i = ft_not_token(s);
		ft_lstadd_back(&head, ft_lstnew(ft_substr(s, 0, i)));
		s += i;
	}
	return (head);
}

int	ft_not_token(const char *s)
{
	int	quote;
	int	i;

	i = 0;
	while (!ft_strchr("(<|>)&;", s[i]) && !ft_isspace(s[i]) && s[i])
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

char* ft_remove_quotes(char* s, int option)
{
	(void)option;
    char* str;
    int quote = 0;
    int i = 0;
    int j = 0;
    int skip = 0;

    str = ft_calloc((ft_strlen(s) - ft_quotes_number(s)) + 1, sizeof(char));
    if (!str)
        return NULL;

    while (s[i])
	{
        if (!quote && (s[i] == '\'' || s[i] == '\"'))
            quote = s[i++];
        else if (quote && s[i] == quote)
		{
            quote = 0;
            i++;
        }
        else if (quote && s[i] != quote)
            str[j++] = s[i++];
        else
		{
            if (skip)
                skip = 0;
            else if (s[i] == '\'' || s[i] == '\"')
                skip = 1;
            str[j++] = s[i++];
        }
    }
    // if (option)
    //     free(s);
    return (str);
}
