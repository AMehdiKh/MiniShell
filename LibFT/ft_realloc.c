/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_realloc.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hahadiou <hahadiou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/15 17:39:55 by hahadiou          #+#    #+#             */
/*   Updated: 2023/05/15 17:40:08 by hahadiou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "libft.h"

void    *ft_realloc(void* ptr, size_t size)
{
	void* new_data;
    
    new_data = NULL;
	if(size)
	{
		if(!ptr)
		{
			return (malloc(size));
		}
		new_data = malloc(size);
		if(new_data)
		{
			ft_memcpy(new_data, ptr, size);
			free(ptr);
		}
	}
	return (new_data);
}
