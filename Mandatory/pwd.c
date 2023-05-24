/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hahadiou <hahadiou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/07 19:02:03 by hahadiou          #+#    #+#             */
/*   Updated: 2023/05/23 23:38:22 by hahadiou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_pwd_builtin(t_shell *shell)
{
    char	wd[PATH_MAX];
	char	*dir;
	
	dir = getcwd(wd, PATH_MAX);
    if (!dir)
	{
        perror("minishell: pwd: ");
        shell->exit_status = EXIT_FAILURE;
        return (-1);
    }
    printf("%s\n", dir);
    shell->exit_status = EXIT_SUCCESS;
    return (EXIT_SUCCESS);
}
