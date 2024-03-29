/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hahadiou <hahadiou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/31 11:30:22 by ael-khel          #+#    #+#             */
/*   Updated: 2023/06/04 15:27:38 by hahadiou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_check_nl(char **reserve)
{
	char	*line;
	char	*tmp;
	int		len;
	int		i;

	i = 0;
	while (*(*reserve + i) != '\n')
		++i;
	line = malloc(i + 2);
	if (!line)
		return (ft_free(reserve));
	ft_strlcpy_gnl(line, *reserve, (i + 2));
	len = ft_strlen_gnl(*reserve + i + 1);
	if (!len)
		ft_free(reserve);
	else
	{
		tmp = malloc(len + 1);
		ft_strlcpy_gnl(tmp, (*reserve + i + 1), (len + 1));
		ft_free(reserve);
		*reserve = malloc(len + 1);
		ft_strlcpy_gnl(*reserve, tmp, (len + 1));
		free(tmp);
	}
	return (line);
}

char	*ft_strjoin_gnl(char **reserve, char *buffer)
{
	char	*tmp;

	tmp = malloc(ft_strlen_gnl(*reserve) + ft_strlen_gnl(buffer) + 1);
	if (!tmp)
		return (ft_free(reserve));
	ft_strlcpy_gnl(tmp, *reserve, ft_strlen_gnl(*reserve) + 1);
	ft_strlcpy_gnl((tmp + ft_strlen_gnl(tmp)), buffer, ft_strlen_gnl(buffer)
		+ 1);
	ft_free(reserve);
	return (tmp);
}

char	*ft_strdup_gnl(char **reserve)
{
	char	*dup;

	if (!**reserve)
		return (ft_free(reserve));
	dup = malloc(ft_strlen_gnl(*reserve) + 1);
	if (!dup)
		return (ft_free(reserve));
	ft_strlcpy_gnl(dup, *reserve, ft_strlen_gnl(*reserve) + 1);
	ft_free(reserve);
	return (dup);
}

char	*get_next_line(int fd)
{
	char		buffer[(-(BUFFER_SIZE < 0) * BUFFER_SIZE) + ((BUFFER_SIZE > 0) \
		* BUFFER_SIZE) + 1];
	static char	*reserve;
	int			nbyte;

	if (fd < 0 || fd == 1 || fd == 2 || BUFFER_SIZE <= 0)
		return (NULL);
	if (!reserve)
	{
		reserve = malloc(1);
		*reserve = '\0';
	}
	while (1)
	{
		nbyte = read(fd, buffer, BUFFER_SIZE);
		if (nbyte < 0)
			return (ft_free(&reserve));
		if (!nbyte && !ft_strchr_gnl(reserve, '\n'))
			return (ft_strdup_gnl(&reserve));
		buffer[nbyte] = '\0';
		if (nbyte)
			reserve = ft_strjoin_gnl(&reserve, buffer);
		if (ft_strchr_gnl(reserve, '\n'))
			return (ft_check_nl(&reserve));
	}
}
