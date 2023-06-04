/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hahadiou <hahadiou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/04 14:15:11 by hahadiou          #+#    #+#             */
/*   Updated: 2023/06/04 14:20:34 by hahadiou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

size_t	exit_status_size(t_shell *shell)
{
	char	*exit_status;
	size_t	size;

	exit_status = ft_litoa(shell->exit_status);
	size = ft_strlen(exit_status);
	free(exit_status);
	return (size);
}

size_t	expand_size(char *input, size_t *i, t_shell *shell)
{
	size_t	var_size;
	size_t	expanded_size;
	char	*var_name;
	char	*var_value;

	(*i)++;
	if (!input[1])
		return (1);
	var_size = 0;
	while (input[var_size + 1] && ft_isalnum(input[var_size + 1]))
		var_size++;
	if (var_size == 0)
		return (0);
	var_name = ft_substr(input, 1, var_size);
	var_value = ft_getenv(var_name, shell->env);
	free(var_name);
	(*i) += var_size;
	if (var_value)
	{
		expanded_size = ft_strlen(var_value);
		if (input[var_size + 1] == '\"')
			expanded_size += 2;
		return (expanded_size);
	}
	return (var_size);
}

int	expanded_size(char *input, t_shell *shell)
{
	size_t	n[2];
	bool	quotes;

	n[0] = 0;
	n[1] = 0;
	quotes = false;
	while (input[n[0]])
	{
		if (input[n[0]] == 39)
			quotes = true;
		if (input[n[0]] == '$' && input[n[0] + 1] == '?' && !quotes)
		{
			n[1] += exit_status_size(shell);
			n[0]++;
		}
		else if (input[n[0]] == '$' && !quotes)
			n[1] += expand_size(&(input[n[0]]), &n[0], shell) - 1;
		else
			n[0]++;
		n[1]++;
	}
	return (n[1]);
}
