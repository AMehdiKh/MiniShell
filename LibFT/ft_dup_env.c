/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_dup_env.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hahadiou <hahadiou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/15 19:31:34 by hahadiou          #+#    #+#             */
/*   Updated: 2023/05/17 16:42:09 by hahadiou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	**ft_dup_env(char **main_env)
{
	char	**env;
	size_t	i;

	i = 0;
	if (!main_env)
		return (NULL);
	while (main_env[i])
		++i;
	env = ft_calloc(i + 1, sizeof(char *));
	if (!env)
	{
		exit(0); // to be changed
	}
	i = 0;
	while (main_env[i])
	{
		env[i] = ft_strdup(main_env[i]);
		++i;
	}
	return (env);
}
