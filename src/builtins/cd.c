/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hahadiou <hahadiou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/05 01:09:26 by hahadiou          #+#    #+#             */
/*   Updated: 2023/04/10 16:03:25 by hahadiou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtins.h"

int	main(int ac, char **av)
{
	char path[4096];

	// No arguments provided, change to home directory
	if (ac == 1)
	{
		const char *home_dir = getenv("HOME");
		if (!home_dir)
		{
			dprintf(2, "cd: HOME not set\n");
			exit(1);
		}
		if (chdir(home_dir) == -1)
		{
			perror("cd");
			exit(1);
		}
	}
	// One argument provided, change to specified directory
	else if (ac == 2)
	{
		const char *dir = av[1];
        
		if (!strcmp(dir, "."))
		{
			// Do nothing, we're already in the current directory
		}
		else if (!strcmp(dir, ".."))
		{
			// Move up one directory level
			if (chdir("..") == -1)
			{
				perror("cd");
				exit(1);
			}
			// Update PWD and OLDPWD environment variables
			if (getcwd(path, 4096))
			{
				setenv("OLDPWD", dir, 1);
				setenv("PWD", path, 1);
			}
			else
			{
				perror("cd");
				exit(1);
			}
		}
		else
		{
			// Change to specified directory
			if (chdir(dir) == -1)
			{
				perror("cd");
				exit(1);
			}
			// Update PWD and OLDPWD environment variables
			if (getcwd(path, 4096))
			{
				setenv("OLDPWD", getenv("PWD"), 1);
				setenv("PWD", path, 1);
			}
			else
			{
				perror("cd");
				exit(1);
			}
		}
	}
	// More than one argument provided, error
	else
	{
		dprintf(2, "cd: too many arguments\n");
		exit(1);
	}
	return (0);
}
