/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hahadiou <hahadiou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/15 22:42:06 by hahadiou          #+#    #+#             */
/*   Updated: 2023/06/04 14:55:20 by hahadiou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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
