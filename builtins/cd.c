/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hahadiou <hahadiou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/24 09:26:41 by ael-khel          #+#    #+#             */
/*   Updated: 2023/05/30 18:34:01 by hahadiou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	update_wd(t_shell *shell)
{
	char	cwd[PATH_MAX];

	getcwd(cwd, PATH_MAX);
	ft_setenv(shell, "PWD", cwd);
}

// static int	cd_oldpwd(char *cwd, t_shell *shell)
// {
// 	char	*owd;

// 	owd = ft_getenv("OLDPWD", shell->env);
// 	if (!owd)
// 	{
// 		ft_dprintf(2, "minishell: cd: OLDPWD not set\n");
// 		return (EXIT_FAILURE);
// 	}
// 	if (chdir(owd) == 0)
// 	{
// 		ft_setenv(shell, "OLDPWD", cwd);
// 		update_wd(shell);
// 		return (EXIT_SUCCESS);
// 	}
// 	return (EXIT_FAILURE);
// }

int	ft_cd_builtin(char *path, t_shell *shell)
{
	char	cwd[PATH_MAX];

	getcwd(cwd, PATH_MAX);
	if (!path)
	{
		ft_setenv(shell, "OLDPWD", cwd);
		chdir(ft_getenv("HOME", shell->env));
		update_wd(shell);
		return (EXIT_SUCCESS);
	}
	// if (ft_strchr(path, '-'))
	// 	return (cd_oldpwd(cwd, shell));
	if (chdir(path) == 0)
	{
		ft_setenv(shell, "OLDPWD", cwd);
		update_wd(shell);
		return (EXIT_SUCCESS);
	}
	ft_dprintf(2, "minishell: cd: %s: No such file or directory\n", path);
	shell->exit_status = EXIT_FAILURE;
	return (EXIT_FAILURE);
}
