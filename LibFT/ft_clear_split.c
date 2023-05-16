/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_clear_split.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hahadiou <hahadiou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/15 22:44:01 by hahadiou          #+#    #+#             */
/*   Updated: 2023/05/15 22:44:10 by hahadiou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "libft.h"

void	*ft_clear_split(char **ptr, size_t x)
{
	while (x--)
		free(ptr[x]);
	free(ptr);
	return (NULL);
}
