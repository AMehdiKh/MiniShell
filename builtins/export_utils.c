/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hahadiou <hahadiou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/04 14:54:47 by hahadiou          #+#    #+#             */
/*   Updated: 2023/06/04 14:55:25 by hahadiou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	sirche(char *s)
{
	while (*s)
	{
		if (*s == '=')
			return (1);
		++s;
	}
	return (0);
}

bool	streq(char *str1, char *str2)
{
	size_t	i;

	if ((str1 && !str2) || (!str1 && str2))
		return (false);
	i = 0;
	while (str1[i] || str2[i])
	{
		if (str1[i] != str2[i])
			return (false);
		i += 1;
	}
	return (true);
}

bool	is_valid_id(char *str)
{
	size_t	i;

	i = 0;
	if (streq(str, "="))
		return (false);
	while (str[i] && str[i] != '=')
	{
		if (ft_isdigit(str[i]) || str[i] == '!' || str[i] == '@'
			|| str[i] == '{' || str[i] == '}' || str[i] == '-')
			return (false);
		i += 1;
	}
	return (true);
}

char	*get_var_name(const char *s, int c)
{
	char	*ptr;
	int		i;

	i = 0;
	ptr = ft_calloc(ft_strlen(s), sizeof(char));
	while (s[i])
	{
		if (s[i] == (char)c)
			return (ptr);
		ptr[i] = s[i];
		++i;
	}
	if (!((char)c))
		return ((char *)s);
	return (NULL);
}
