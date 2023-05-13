/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dollar_sign.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ael-khel <ael-khel@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/13 18:03:05 by ael-khel          #+#    #+#             */
/*   Updated: 2023/05/13 20:22:33 by ael-khel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <stddef.h>

int	ft_check_dollar(char *s)
{
	int		quote;

	while (*s)
	{
		quote = 0;
		if (*s == 39)
			quote = *s++;
		while (quote == 39 && *s != quote)
			++s;
		if (*s == '$')
			return (1);
		++s;
	}
	return (0);
}

char	*ft_expander(t_shell *shell, char *s)
{
	char	*str;
	int		quote;
	size_t	i;
	size_t	j;

	if (!ft_check_dollar(s))
		return (s);
	i = 0;
	j = 0;
	while (s[i])
	{
		quote = 0;
		if (s[i] == 39 || s[i] == 34)
			quote = s[i++];
		while (quote == 39 && s[i++] != quote)
			;
		if (s[i] == '$')
		{
			str = ft_substr(s, j, i);
			j = i;
			str = ft_strjoin(str, ft_dollar_expansion(s + i, i), 4);
		}
	}
	return (0);
}

char	*ft_dollar_expansion(t_shell *shell, char *s, int *i, int quote)
{
	if (*(s + 1) == '?')
		return (ft_itoa(shell->exit_status));
	if (!ft_isalpha(*(s + 1)))
		return (ft_strdup(s));
	
}
