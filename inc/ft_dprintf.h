/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_dprintf.h                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ael-khel <ael-khel@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/16 11:09:00 by ael-khel          #+#    #+#             */
/*   Updated: 2023/04/16 11:15:40 by ael-khel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_DPRINTF_H
# define FT_DPRINTF_H

# include <unistd.h>
# include <stdarg.h>

typedef struct s_dprintf
{
	va_list	ap;
	int		fd;
	int		len;
	int		spec;
}	t_dprintf;

int			ft_dprintf(int fd, const char *format, ...);

const char	*ft_check_spec(const char *format, t_dprintf *tab);

void		ft_putchar(char c, t_dprintf *tab);
void		ft_putstr(char *s, t_dprintf *tab);
void		ft_putnbr(long n, t_dprintf *tab);
void		ft_puthex(unsigned long n, t_dprintf *tab);

#endif