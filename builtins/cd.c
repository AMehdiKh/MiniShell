/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hahadiou <hahadiou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/24 09:26:41 by ael-khel          #+#    #+#             */
/*   Updated: 2023/05/19 20:28:44 by hahadiou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "minishell.h"

static void	update_wd(t_shell *shell)
{
	char	cwd[PATH_MAX];

	getcwd(cwd, PATH_MAX);
	printf("cwd: [%s]\n", cwd);
	ft_setenv(shell, "PWD", cwd);
}

int	ft_cd_builtin(char	*path, t_shell *shell)
{
	char	cwd[PATH_MAX];

	getcwd(cwd, PATH_MAX);
	if (!path || ft_strchr(path, '~'))
	{
		printf("owd: [%s]\n", cwd);
		ft_setenv(shell, "OLDPWD", cwd);
		chdir(ft_getenv("HOME", shell->env, -1));
		update_wd(shell);
		return (EXIT_SUCCESS);
	}
	if (chdir(path) == 0)
	{
		printf("owd: [%s]\n", cwd);
		ft_setenv(shell, "OLDPWD", cwd);
		update_wd(shell);
		return (EXIT_SUCCESS);
	}
	ft_dprintf(2, "minishell: cd: %s: No such file or directory", path);
	return (EXIT_FAILURE);
}

// int main(int ac, char **av, char **envp)
// {
// 	t_shell shell[1];
// 	shell->env = envp;
// 	ft_cd("../inc", shell);
// 	printf("\n   ENV   \n");
// 	for (int i = 0; shell->env[i]; i++)
// 		printf("%s\n", shell->env[i]);
// 	return (0);
// }
