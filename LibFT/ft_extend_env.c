/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_extend_env.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hahadiou <hahadiou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/15 23:04:52 by hahadiou          #+#    #+#             */
/*   Updated: 2023/05/15 23:05:02 by hahadiou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	**extend_env(char **envp, char *newstr)
{
	size_t	len;
	size_t	i;
	char	**out;

	if (!newstr)
		return (envp);
	len = ft_count_strings(envp);
	out = (char **)malloc(sizeof(char *) * (len + 2));
	if (!out)
	{
		ft_clear_split(envp, ft_count_strings(envp));
		return (envp);
	}
	i = 0;
	while (i < len)
	{
		out[i] = ft_strdup(envp[i]);
		if (!out[i])
		{
			ft_clear_split(envp, ft_count_strings(envp));
			ft_clear_split(out, ft_count_strings(out));
			return (NULL);
		}
		i++;
	}
	out[i] = ft_strdup(newstr);
	if (!out[i])
	{
		ft_clear_split(envp, ft_count_strings(envp));
		ft_clear_split(out, ft_count_strings(out));
		return (NULL);
	}
	out[i + 1] = NULL;
	ft_clear_split(envp, ft_count_strings(envp));
	return (out);
}
