/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hahadiou <hahadiou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/14 22:08:45 by hahadiou          #+#    #+#             */
/*   Updated: 2023/05/15 22:39:06 by hahadiou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int ft_unset(char *var, char **envp)
{
	char	**temp_env;
	size_t	var_len;

	var_len = ft_strlen(var);
	while (*envp)
	{
		if (!ft_strncmp(*envp, var, var_len))
		{
			temp_env = envp;
			//free(temp_env[0]);
            while (*temp_env)
			{
				temp_env[0] = temp_env[1];
				temp_env++;
			}
		}
		envp++;
	}
	return (0);
}

char	**ft_dup_env(char **main_env)
{
	char	**env;
	size_t	i;

	i = 0;
	while (main_env[i])
		++i;
	env = ft_calloc(i + 1, sizeof(char *));
	if (!env)
	{
		exit(0);
	}
	i = 0;
	while (main_env[i])
	{
		env[i] = ft_strdup(main_env[i]);
		++i;
	}
	return (env);
}

int main(int ac, char **av, char **envp)
{
	t_shell shell[1];

	shell->env = ft_dup_env(envp);
	char* var_name = av[1];
	//printf("Before unset:\n");
    //for (char** env = envp; *env != NULL; env++)
	// while (*shell->env)
	// {
    //     printf("%s\n", *shell->env);
	// 	shell->env++;
	// }
	ft_unset(var_name, shell->env);
	printf("After unset:\n");
    // for (char** env = envp; *env != NULL; env++)
	while (*shell->env)
	{
        printf("%s\n", *shell->env);
		shell->env++;
	}
	return (0);
}