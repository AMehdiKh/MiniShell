/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_get_env.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hahadiou <hahadiou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/15 23:11:11 by hahadiou          #+#    #+#             */
/*   Updated: 2023/06/15 14:06:44 by hahadiou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	ft_memcmp(const void *s1, const void *s2, size_t n)
{
	const unsigned char	*p1;
	const unsigned char	*p2;
	size_t				i;

	p1 = (const unsigned char *)s1;
	p2 = (const unsigned char *)s2;
	i = 0;
	while (i < n)
	{
		if (p1[i] != p2[i])
		{
			if (p1[i] < p2[i])
				return (-1);
			else
				return (1);
		}
		i++;
	}
	return (0);
}

char	*ft_getenv(char *var, char **envp)
{
	size_t		var_len;
	size_t		i;
	const char	*env_var;
	char		*eq_pos;

	var_len = ft_strlen(var);
	i = 0;
	while (envp && envp[i])
	{
		env_var = envp[i];
		eq_pos = ft_strchr(env_var, '=');
		if (eq_pos && (size_t)(eq_pos - env_var) == var_len && ft_memcmp(var,
				env_var, var_len) == 0)
			return (ft_strdup(eq_pos + 1));
		i++;
	}
	return (NULL);
}
