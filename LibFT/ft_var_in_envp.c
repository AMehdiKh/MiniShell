/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_var_in_envp.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hahadiou <hahadiou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/15 23:06:51 by hahadiou          #+#    #+#             */
/*   Updated: 2023/05/15 23:07:01 by hahadiou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	find_var_in_envp(const char *var, char **env)
{
	int		found;
	size_t	var_len;
	size_t	i;

	found = -1;
	var_len = ft_strlen(var);
	i = 0;
	while (env[i])
	{
		if (ft_strncmp(var, env[i], var_len) == 0 && env[i][var_len] == '=')
		{
			found = 1;
			break ;
		}
		i++;
	}
	return (found);
}
