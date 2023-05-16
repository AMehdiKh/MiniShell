/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hahadiou <hahadiou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/14 23:08:45 by hahadiou          #+#    #+#             */
/*   Updated: 2023/05/16 22:26:47 by hahadiou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "minishell.h"

static size_t	exit_status_size(t_shell *shell)
{
	char	*exit_status;
	size_t	size;

	exit_status = ft_litoa(shell->exit_status);
	size = ft_strlen(exit_status);
	free(exit_status);
	return (size);
}

static size_t	expand_size(char *input, size_t *i, t_shell *shell)
{
	size_t	var_size;
	char	*var_name;
	char	*var_value;

	*i += 1;
	if (!input[1])
		return (1);
	var_size = 0;
	while (input[var_size + 1] && ft_isalnum(input[var_size + 1]))
		var_size++;
	if (var_size == 0)
		return (0);
	var_name = ft_substr(input, 1, var_size);
	var_value = ft_getenv(var_name, shell->env, -1);
	free(var_name);
	*i += var_size;
	if (!var_value)
		return (0);
	return (ft_strlen(var_value));
}

static int	expanded_size(char *input, t_shell *shell)
{
	size_t	n[2];
	bool	quotes; // quotes [0] is single quotes otherwize is double quotes

	n[0] = 0; // n[0] is the variable to loop through the input
	n[1] = 0; // n[1] is the size of expanded variable
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

static size_t	expand_vars(char *expanded, char *input, size_t *i, t_shell *shell)
{
	char	*val;
	size_t	n[3];

	n[0] = 0; // this is the variable of i to loop through input
	n[1] = 0; // this is a variable to be used for copy value from val to expanded
	n[2] = 0; // this is the size of expanded variable to more to next variable
	*i += 1;
	if (!input[*i])
	{
		expanded[0] = '$';
		return (1);
	}
	while (input[*i + n[2]] == '_' || ft_isalnum(input[*i + n[2]]))
		n[2]++;
	val = ft_getenv(ft_substr(input, *i, n[2]), shell->env, -1);
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
		j += 1;
	}
	free(exit_status);
	return (j);
}

char	*expander(char *input, t_shell *shell)
{
	size_t	n[2];
	char	*expanded;
	bool	quotes;

	n[0] = 0;
	n[1] = 0;
	quotes = false;
	expanded = (char *)ft_calloc((expanded_size(input, shell) + 1) , sizeof(char));
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
	free(input);
	return (expanded);
}

// int main(int ac, char **av, char **envp)
// {
//     t_shell shell[1];

// 	shell->env = ft_dup_env(envp);
//     char input[] = "$HOME";
// 	char *expanded = expander(input, shell);
// 	size_t	i = 0;
// 	//expand_vars(expanded, input, &i, shell);
//     //int size = expanded_size(input, shell);
// 	printf("expanded: %s\n", expanded);
//     free(shell->env);
//     return 0;
// }

 