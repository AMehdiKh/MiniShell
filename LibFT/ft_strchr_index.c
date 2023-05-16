/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_stchr_index.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hahadiou <hahadiou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/15 17:18:01 by hahadiou          #+#    #+#             */
/*   Updated: 2023/05/15 17:18:16 by hahadiou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "libft.h"

int find_set_char_index(const char *s, char *set)
{
    const char  *p;
    
    if (!s)
    {
        return -1;
    }
    p = s;
    while (*p)
    {
        if (ft_strchr(set, *p))
            return (p - s);
        p++;
    }
    return (-1);
}
