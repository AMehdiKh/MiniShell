/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_dprintf.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ael-khel <ael-khel@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/18 02:44:18 by ael-khel          #+#    #+#             */
/*   Updated: 2023/04/18 02:44:30 by ael-khel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

const char	*ft_check_spec(const char *format, t_dprintf *tab)
{
	tab->spec = *format;
	if (*format == '%')
		ft_putchar('%', tab);
	else if (*format == 'c')
		ft_putchar(va_arg(tab->ap, int), tab);
	else if (*format == 's')
		ft_putstr(va_arg(tab->ap, char *), tab);
	else if (*format == 'u')
		ft_putnbr(va_arg(tab->ap, unsigned int), tab);
	else if (*format == 'd' || *format == 'i')
		ft_putnbr(va_arg(tab->ap, int), tab);
	else if (*format == 'x' || *format == 'X')
		ft_puthex(va_arg(tab->ap, unsigned int), tab);
	else if (*format == 'p')
	{
		ft_putstr("0x", tab);
		ft_puthex(va_arg(tab->ap, unsigned long), tab);
	}
	if (*format)
		++format;
	return (format);
}

int	ft_dprintf(int fd, const char *format, ...)
{
	t_dprintf	tab[1];

	tab->fd = fd;
	tab->len = 0;
	va_start(tab->ap, format);
	while (*format)
	{
		if (*format == '%')
			format = ft_check_spec(format + 1, tab);
		else
			tab->len += write(fd, format++, 1);
	}
	va_end(tab->ap);
	return (tab->len);
}
