/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_count_strings.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hahadiou <hahadiou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/15 22:45:38 by hahadiou          #+#    #+#             */
/*   Updated: 2023/05/15 22:47:10 by hahadiou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "libft.h"

size_t ft_count_strings(char **strings)
{
    size_t count;
    
    count = 0;
    while (strings[count])
        count++;
    return (count);
}
