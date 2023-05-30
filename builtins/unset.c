/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hahadiou <hahadiou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/14 22:08:45 by hahadiou          #+#    #+#             */
/*   Updated: 2023/05/30 05:20:18 by hahadiou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// to be fixed
int	ft_unset_builtin(char *var, char **envp)
{
	char	**temp_env;
	size_t	var_len;

	var_len = ft_strlen(var);
	while (*envp)
	{
		if (!ft_strncmp(*envp, var, var_len))
		{
			temp_env = envp;
			// free(temp_env[0]);
			while (*temp_env)
			{
				temp_env[0] = temp_env[1];
				temp_env++;
			}
		}
		envp++;
	}
	return (0);
}
