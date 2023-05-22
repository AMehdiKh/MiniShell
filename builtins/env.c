/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hahadiou <hahadiou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/12 19:33:10 by hahadiou          #+#    #+#             */
/*   Updated: 2023/05/19 19:48:09 by hahadiou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int ft_env_builtin(char **av, t_shell *shell)
{
    int i;

    i = 0;
    if (av[1])
    {
        ft_dprintf(2, "env: %s: No such file or directory\n", av[1]);
        shell->exit_status = 127;
        return (EXIT_FAILURE);
    }
    while (shell->env[i])
    {
        if (strchr(shell->env[i], '='))
            printf("%s\n", shell->env[i]);
        i++;
    }
    return (EXIT_SUCCESS);
}

// int main(int ac, char **av, char **envp)
// {
//     printf("%d\n", env(av, envp));
//     return (0);
// }
