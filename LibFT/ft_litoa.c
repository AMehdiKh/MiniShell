/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_litoa.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hahadiou <hahadiou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/02 00:14:26 by hahadiou          #+#    #+#             */
/*   Updated: 2023/05/16 20:22:17 by hahadiou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_litoa(int n)
{
	unsigned int	nb;
	char			*ret;
	int				len;

	nb = -n * (n < 0) + n * (n > 0);
	len = 0 + 1 * (n <= 0);
	while (nb)
	{
		nb /= 10;
		++len;
	}
	ret = (char *)malloc((len + 1) * sizeof(char));
	if (!ret)
		return (NULL);
	ret[len] = 0;
	nb = -n * (n < 0) + n * (n > 0);
	ret[0] = '-';
	while (nb)
	{
		ret[--len] = '0' + nb % 10;
		nb /= 10;
	}
	ret[0] = '0' * (n == 0) + ret[0] * (n != 0);
	return (ret);
}
