/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hahadiou <hahadiou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/15 22:42:06 by hahadiou          #+#    #+#             */
/*   Updated: 2023/05/30 17:45:10 by hahadiou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

bool	streq(char *str1, char *str2)
{
	size_t	i;

	if ((str1 && !str2) || (!str1 && str2))
		return (false);
	i = 0;
	while (str1[i] || str2[i])
	{
		if (str1[i] != str2[i])
			return (false);
		i += 1;
	}
	return (true);
}

bool	is_valid_id(char *str)
{
	size_t	i;

	i = 0;
	if (streq(str, "="))
		return (false);
	while (str[i] && str[i] != '=')
	{
		if (ft_isdigit(str[i]) || str[i] == '!' || str[i] == '@'
			|| str[i] == '{' || str[i] == '}' || str[i] == '-')
			return (false);
		i += 1;
	}
	return (true);
}

char	*get_var_name(const char *s, int c)
{
	char	*ptr;
	int		i;

	i = 0;
	ptr = ft_calloc(ft_strlen(s), sizeof(char));
	while (s[i])
	{
		if (s[i] == (char)c)
			return (ptr);
		ptr[i] = s[i];
		++i;
	}
	if (!((char)c))
		return ((char *)s);
	return (NULL);
}

int	single_export(t_shell *shell)
{
	size_t	i;

	i = 0;
	while (shell->env[i])
	{
		ft_dprintf(STDOUT_FILENO, "declare -x %s=\"%s\"\n",
			get_var_name(shell->env[i], '='), ft_strchr(shell->env[i], '=')
			+ 1);
		i++;
	}
	return (EXIT_SUCCESS);
}

int	export_bad_identifier(char *identifier)
{
	ft_dprintf(STDERR_FILENO,
		"minishell: export: `%s': not a valid identifier\n", identifier);
	return (EXIT_FAILURE);
}

int	sirche(char *s)
{
	while (*s)
	{
		if (*s == '=')
			return (1);
		++s;
	}
	return (0);
}

int	ft_export_builtin(t_shell *shell, char **av)
{
	size_t	arg_count;
	size_t	i;

	i = 0;
	arg_count = ft_count_strings(av);
	if (arg_count == 1)
		return (single_export(shell));
	shell->exit_status = EXIT_SUCCESS;
	while (av[i])
	{
		if (!is_valid_id(av[i]))
			shell->exit_status = export_bad_identifier(av[i]);
		else if (sirche(av[i]) == 1)
			ft_setenv(shell, get_var_name(av[i], '='), ft_strchr(av[i], '=')
				+ 1);
		i++;
	}
	return (shell->exit_status);
}
