/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_set_env.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hahadiou <hahadiou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/15 23:11:11 by hahadiou          #+#    #+#             */
/*   Updated: 2023/06/04 15:05:24 by hahadiou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*create_entry(char *variable, char *value, size_t var_len,
		size_t value_len, size_t entry_len)
{
	char *entry = (char *)malloc(sizeof(char) * entry_len);
	if (!entry)
		return (NULL);
	strncpy(entry, variable, var_len);
	entry[var_len] = '=';
	strncpy(entry + var_len + 1, value, value_len);
	entry[entry_len - 1] = '\0';
	return (entry);
}

void	ft_setenv(t_shell *shell, char *variable, char *value)
{
	size_t	i;
	size_t	var_len;
	size_t	value_len;
	size_t	entry_len;
	char	*equal_sign;
	char	*entry;

	if (!shell || !variable)
		return ;
	i = 0;
	var_len = ft_strlen(variable);
	value_len = ft_strlen(value);
	entry_len = var_len + value_len + 2;
	while (shell->env[i])
	{
		if (ft_strnstr(shell->env[i], variable, ft_strlen(variable)) == shell->env[i])
		{
			equal_sign = ft_strchr(shell->env[i], '=');
			if (equal_sign && (equal_sign - shell->env[i] == (long)var_len))
			{
				entry = create_entry(variable, value, var_len, value_len,
					entry_len);
				if (!entry)
					return ;
				shell->env[i] = entry;
			}
			return ;
		}
		i++;
	}
	entry = create_entry(variable, value, var_len, value_len, entry_len);
	if (!entry)
		return ;
	shell->env = extend_env(shell->env, entry);
}
