/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_dup_env.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hahadiou <hahadiou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/15 19:31:34 by hahadiou          #+#    #+#             */
/*   Updated: 2023/05/15 19:31:49 by hahadiou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "libft.h"

char	**ft_dup_env(char **main_env)
{
	char	**env;
	size_t	i;

	i = 0;
	while (main_env[i])
		++i;
	env = ft_calloc(i + 1, sizeof(char *));
	if (!env)
	{
		exit(0);		
	}
	i = 0;
	while (main_env[i])
	{
		env[i] = ft_strdup(main_env[i]);
		++i;
	}
	return (env);
}
