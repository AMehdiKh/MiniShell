/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hahadiou <hahadiou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/14 23:08:45 by hahadiou          #+#    #+#             */
/*   Updated: 2023/06/04 14:20:00 by hahadiou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static size_t	expand_vars(char *expanded, char *input, size_t *i,
		t_shell *shell)
{
	char	*val;
	size_t	n[3];

	n[0] = 0;
	n[1] = 0;
	n[2] = 0;
	*i += 1;
	while (input[*i + n[2]] == '_' || ft_isalnum(input[*i + n[2]]))
		n[2]++;
	if (ft_isdigit(input[*i]))
	{
		*i += 1;
		while (input[*i] && !ft_isdigit(input[*i]))
			*i += 1;
		while (input[*i])
			expanded[n[1]++] = input[(*i)++];
		return (n[1]);
	}
	val = ft_getenv(ft_substr(input, *i, n[2]), shell->env);
	*i += n[2];
	if (!val)
		return (0);
	while (val[n[0]])
		expanded[n[1]++] = val[n[0]++];
	return (n[1]);
}

static size_t	expand_exit_status(char *expanded, size_t *i, t_shell *shell)
{
	char	*exit_status;
	size_t	j;

	*i += 2;
	exit_status = ft_litoa(shell->exit_status);
	j = 0;
	while (exit_status[j])
	{
		expanded[j] = exit_status[j];
		++j;
	}
	free(exit_status);
	return (j);
}

char	*ft_expander(char *input, t_shell *shell)
{
	size_t	n[2];
	char	*expanded;
	bool	quotes;

	n[0] = 0;
	n[1] = 0;
	quotes = false;
	expanded = ft_calloc((expanded_size(input, shell) + 1), sizeof(char));
	while (input[n[0]])
	{
		if (input[n[0]] == 39)
			quotes = true;
		if (input[n[0]] == '$' && input[n[0] + 1] == '?' && !quotes)
			n[1] += expand_exit_status(&(expanded[n[1]]), &n[0], shell);
		else if (input[n[0]] == '$' && !quotes)
			n[1] += expand_vars(&(expanded[n[1]]), input, &n[0], shell);
		else
			expanded[n[1]++] = input[n[0]++];
	}
	return (expanded);
}
