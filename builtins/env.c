/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ael-khel <ael-khel@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/12 19:33:10 by hahadiou          #+#    #+#             */
/*   Updated: 2023/06/05 22:57:36 by ael-khel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <stdlib.h>

int	ft_env_builtin(char **av, t_shell *shell)
{
	int	i;

	i = 0;
	if (av[1])
	{
		ft_dprintf(2, "env: %s: No such file or directory\n", av[1]);
		shell->exit_status = 127;
		return (127);
	}
	while (shell->env[i])
	{
		if (strchr(shell->env[i], '='))
			ft_dprintf(1, "%s\n", shell->env[i]);
		i++;
	}
	shell->exit_status = 0;
	return (EXIT_SUCCESS);
}
