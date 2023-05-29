/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hahadiou <hahadiou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/24 22:38:07 by ael-khel          #+#    #+#             */
/*   Updated: 2023/05/18 16:54:43 by hahadiou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef BUILTIN_H
# define BUILTIN_H

# include "libft.h"

void	ft_exit_builtin(int ac, char **av);
int	    ft_echo_builtin(size_t ac, char **av);
int     env(char **av, char **env);
int     ft_unset(char *var, char **env);

#endif