/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hahadiou <hahadiou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/06 18:07:10 by hahadiou          #+#    #+#             */
/*   Updated: 2023/04/10 16:04:22 by hahadiou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtins.h"

int main(int ac, char **av, char **envp) 
{
    (void)ac;
    (void)av;
    while (*envp) 
    {
        printf("%s\n", *envp);
        envp++;
    }
    return (0);
}
