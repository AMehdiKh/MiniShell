/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hahadiou <hahadiou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/14 23:08:45 by hahadiou          #+#    #+#             */
/*   Updated: 2023/05/16 01:38:20 by hahadiou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "minishell.h"

char	*expand_path(char *str, int i, int quotes[2], char *var)
{
	char	*path;
	char	*aux;

	quotes[0] = 0;
	quotes[1] = 0;
	while (str && str[++i])
	{
		quotes[0] = (quotes[0] + (!quotes[1] && str[i] == '\'')) % 2;
		quotes[1] = (quotes[1] + (!quotes[0] && str[i] == '\"')) % 2;
		if (!quotes[0] && !quotes[1] && str[i] == '~' && (i == 0 || \
			str[i - 1] != '$'))
		{
			aux = ft_substr(str, 0, i);
			path = ft_strjoin(aux, var, 1);
			aux = ft_substr(str, i + 1, ft_strlen(str));
			free(str);
			str = ft_strjoin(path, aux, 4);
			return (expand_path(str, i + ft_strlen(var) - 1, quotes, var));
		}
	}
	free(var);
	return (str);
}

static char	*get_substr_var(char *str, int i, t_shell *shell)
{
	char	*aux;
	int		pos;
	char	*path;
	char	*var;

	pos = find_set_char_index(&str[i], SPECIFIER) + (ft_strchr("$?", str[i]) != 0);
	if (pos == -1)
		pos = ft_strlen(str) - 1;
	aux = ft_substr(str, 0, i - 1);
	var = ft_getenv(&str[i], shell->env, find_set_char_index(&str[i], SPECIFIER));
	if (!var && str[i] == '?')
		var = ft_itoa(shell->exit_status);
	path = ft_strjoin(aux, var, 1);
	aux = ft_strjoin(path, &str[i + pos], 1);
	free(var);
	free(str);
	return (aux);
}

char	*expand_vars(char *str, int i, int quotes[2], t_shell *shell)
{
	quotes[0] = 0;
	quotes[1] = 0;
	while (str && str[++i])
	{
		quotes[0] = (quotes[0] + (!quotes[1] && str[i] == '\'')) % 2;
		quotes[1] = (quotes[1] + (!quotes[0] && str[i] == '\"')) % 2;
		if (!quotes[0] && str[i] == '$' && str[i + 1] && \
			((find_set_char_index(&str[i + 1], SPECIFIER) && !quotes[1]) || \
			(find_set_char_index(&str[i + 1], "/~%^{}:;\"") && quotes[1])))
			return (expand_vars(get_substr_var(str, ++i, shell), -1, \
				quotes, shell));
	}
	return (str);
}