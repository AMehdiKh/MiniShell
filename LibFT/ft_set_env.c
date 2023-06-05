/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_set_env.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hahadiou <hahadiou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/15 23:11:11 by hahadiou          #+#    #+#             */
/*   Updated: 2023/06/04 17:46:57 by hahadiou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*create_entry(char *variable, char *value, size_t i)
{
	size_t	var_len;
	size_t	value_len;
	size_t	entry_len;
	char	*entry;
	char	*ptr;

	var_len = ft_strlen(variable);
	value_len = ft_strlen(value);
	entry_len = var_len + value_len + 2;
	entry = (char *)malloc(entry_len);
	if (!entry)
		return (NULL);
	ptr = entry;
	i = -1;
	while (++i < var_len)
		*ptr++ = variable[i];
	*ptr++ = '=';
	i = -1;
	while (++i < value_len)
		*ptr++ = value[i];
	*ptr = '\0';
	return (entry);
}

void	update_env_entry(t_shell *shell, char *variable, char *value, int i)
{
	char	*equal_sign;
	size_t	var_len;
	char	*entry;

	equal_sign = ft_strchr(shell->env[i], '=');
	var_len = ft_strlen(variable);
	if (equal_sign && (equal_sign - shell->env[i] == (long)var_len))
	{
		entry = create_entry(variable, value, -1);
		if (!entry)
			return ;
		free(shell->env[i]);
		shell->env[i] = entry;
	}
}

void	ft_setenv(t_shell *shell, char *variable, char *value)
{
	size_t	i;
	char	*entry;

	if (!shell || !variable)
		return ;
	i = 0;
	while (shell->env[i])
	{
		if (ft_strncmp(shell->env[i], variable, ft_strlen(variable)) == 0)
		{
			update_env_entry(shell, variable, value, i);
			return ;
		}
		i++;
	}
	entry = create_entry(variable, value, -1);
	if (!entry)
		return ;
	shell->env = extend_env(shell->env, entry);
}
