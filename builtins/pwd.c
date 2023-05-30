/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hahadiou <hahadiou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/07 19:02:03 by hahadiou          #+#    #+#             */
/*   Updated: 2023/05/30 05:12:18 by hahadiou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_pwd_builtin(void)
{
	char	wd[PATH_MAX];
	char	*dir;

	dir = getcwd(wd, PATH_MAX);
	if (!dir)
	{
		perror("minishell: pwd: ");
		return (-1);
	}
	ft_dprintf(1, "%s\n", dir);
	return (EXIT_SUCCESS);
}
