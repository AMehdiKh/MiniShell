/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hahadiou <hahadiou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/15 22:42:06 by hahadiou          #+#    #+#             */
/*   Updated: 2023/05/19 19:48:02 by hahadiou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "minishell.h"

int	ft_export_builtin(t_shell *shell, char **av)
{
	size_t  arg_count;
    size_t  i;
    
    arg_count = ft_count_strings(av);
    i = 0;
	if (arg_count >= 2)
	{
        while (i < arg_count)
		{
			int	pos = find_var_in_envp(av[i], shell->env);
			if (pos >= 0)
			{
				free(shell->env[pos]);
				shell->env[pos] = ft_strdup(av[i]);
			}
			else
			{
				shell->env = extend_env(shell->env, av[i]);
			}
            i++;
		}
	}
	return (0);
}
