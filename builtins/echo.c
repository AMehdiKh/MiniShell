/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ael-khel <ael-khel@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/25 03:53:49 by ael-khel          #+#    #+#             */
/*   Updated: 2023/04/25 06:23:51 by ael-khel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtin.h"

int	ft_echo_builtin(size_t ac, char **av)
{
	size_t	i;
	int		n;

	n = (ft_strncmp("-n", av[1], 3) == 0);
	i = 1 + n;
	while (i < ac)
	{
		write(1, av[i], ft_strlen(av[i]));
		if (i != (ac - 1))
			write(1, " ", 1);
		++i;
	}
	if (!n)
		write(1, "\n", 1);
	return (EXIT_SUCCESS);
}
