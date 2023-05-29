/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hahadiou <hahadiou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/24 09:26:41 by ael-khel          #+#    #+#             */
/*   Updated: 2023/05/19 19:48:05 by hahadiou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/**
* ?	https://www.gnu.org/savannah-checkouts/gnu/bash/manual/bash.html#index-exit
* *	exit [n] with n	   = the exit status is first byte of n (0 - 255).
* *	exit [n] without n = the exit status is that of the last command executed.
* TODO : perhaps that one global var that we can use, has something todo here.
* !	exit [n] n not num = the exit status is 2 to indicate incorrect usage.
* !	exit [n] many args = the exit status is 1 to indicate many aguments.
**/

#include "minishell.h"

void	ft_exit_builtin(char *status, char *av, int ac)
{
	char		*s;
	int			result;
	int			sign;

	result = 0;
	sign = 1;
	s = status;
	if (*s == '+' || *s == '-')
		if (*s++ == '-')
			sign = -sign;
	while (ft_isdigit(*s))
		result = (result * 10) + (*s++ - 48);
	if (*s)
	{
		free(status);
		ft_dprintf(2, "minishell: exit: %s: numeric argument required\n", av);
		exit(2);
	}
	free (status);
	if (ac > 2)
	{
		ft_dprintf(2, "minishell: exit: too many arguments\n");
		exit(1);
	}
	exit ((unsigned char)(result * sign));
}

void	ft_exit_builtin(int ac, char **av)
{
	if (ac > 1)
		ft_exit(ft_strtrim(av[1], " "), av[1], ac);
	exit(EXIT_SUCCESS); // incorrect line
}
