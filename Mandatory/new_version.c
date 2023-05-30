/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   new_version.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hahadiou <hahadiou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/28 21:58:51 by hahadiou          #+#    #+#             */
/*   Updated: 2023/05/29 15:18:25 by hahadiou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void    exec_cmd(t_lexer *curr, t_shell *shell)
{
    if (curr->type == PIPE)
        exec_pipe(curr, shell);
    else if (curr->type == CMD || curr->type == BUILTIN)
        exec_executable(curr, shell);
    else
        exec_redirections(curr, shell);
    exit(shell->exit_status);
}

void    exec_type(t_lexer *lexer, t_shell *shell)
{
    int status;

    // if (lstlexer_size(shell->lexer) == 1)
    // {
    //     exec_executables(lexer, shell);
    // }
    if (fork() == 0)
        exec_cmd(lexer, shell);
    waitpid(-1, &status, 0);
}
