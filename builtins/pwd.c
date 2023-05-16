/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hahadiou <hahadiou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/07 19:02:03 by hahadiou          #+#    #+#             */
/*   Updated: 2023/05/15 22:18:53 by hahadiou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	pwd(struct s_shell *shell)
{
    char	wd[PATH_MAX];
	char	*dir;
	
	dir = getcwd(wd, sizeof(wd));
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

// int main(void)
// {
//     t_shell shell;
//     int ret;
    
//     shell.lexer = NULL;
//     shell.env = NULL;
//     shell.line = NULL;
//     shell.i = 0;
//     shell.j = 0;
//     shell.lexer_status = 0;
//     shell.exit_status = 0;
//     ret = pwd(&shell);
//     if (ret == -1)
//     {
//         printf("ft_pwd failed\n");
//         exit(EXIT_FAILURE);
//     }
//     exit(EXIT_SUCCESS);
// }
