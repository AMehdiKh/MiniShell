/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hahadiou <hahadiou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/05 00:57:45 by hahadiou          #+#    #+#             */
/*   Updated: 2023/04/10 16:09:00 by hahadiou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtins.h"

int main(int ac, char **av) 
{
    int i;
    for (i = 1; i < ac; i++)
    {
        if (!strncmp(av[i], "-n", 2))
            break ;
        printf("%s", av[i]);
        if (i < ac - 1)
           printf(" ");
    }
  // if (ac > 1 &&  av[2] && strncmp(av[2], "-n", 2))
    //  dprintf(1, "\n");
    return 0;
}
