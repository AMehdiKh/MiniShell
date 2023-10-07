/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ael-khel <ael-khel@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/24 09:26:41 by ael-khel          #+#    #+#             */
/*   Updated: 2023/06/06 00:59:44 by ael-khel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_exit(char *status, char *av, int ac)
{
	char	*s;
	long	result;
	int		sign;

	result = 0;
	sign = 1;
	s = status;
	if (*s == '+' || *s == '-')
		if (*s++ == '-')
			sign = -sign;
	while (ft_isdigit(*s))
		result = (result * 10) + (*s++ - 48);
	if (*s || result > LLONG_MAX)
	{
		free(status);
		ft_dprintf(2, "minishell: exit: %s: numeric argument required\n", av);
		exit(255);
	}
	free(status);
	if (ac > 2)
	{
		ft_dprintf(2, "minishell: exit: too many arguments\n");
		exit(EXIT_FAILURE);
	}
	exit((unsigned char)(result * sign));
}

void	ft_exit_builtin(int ac, char *av, t_shell *shell)
{
	if (ac > 1)
		ft_exit(ft_strtrim(av, " \t"), av, ac);
	exit(shell->exit_status);
}
