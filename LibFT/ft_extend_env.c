/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_extend_env.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hahadiou <hahadiou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/15 23:04:52 by hahadiou          #+#    #+#             */
/*   Updated: 2023/06/04 16:19:21 by hahadiou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static void	allocation_failed(char **envp, char **out)
{
	ft_clear_split(envp, ft_count_strings(envp));
	ft_clear_split(out, ft_count_strings(out));
}

static char	**allocate_new_env(char **envp, size_t len, char *newstr)
{
	char	**out;
	size_t	i;

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
			return (allocation_failed(envp, out), NULL);
		i++;
	}
	out[i] = ft_strdup(newstr);
	if (!out[i])
		return (allocation_failed(envp, out), NULL);
	out[i + 1] = NULL;
	ft_clear_split(envp, ft_count_strings(envp));
	return (out);
}

char	**extend_env(char **envp, char *newstr)
{
	size_t	len;

	if (!newstr)
		return (envp);
	len = ft_count_strings(envp);
	return (allocate_new_env(envp, len, newstr));
}
