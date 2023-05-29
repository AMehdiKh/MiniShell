/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_set_env.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hahadiou <hahadiou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/15 23:11:11 by hahadiou          #+#    #+#             */
/*   Updated: 2023/05/19 19:52:11 by hahadiou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "minishell.h"

void ft_setenv(t_shell *shell, char *variable, char *value)
{
    size_t  i;
    size_t  var_len;
    size_t  value_len;
    size_t  entry_len;
    char    *equal_sign;
    char    *entry;
    
	if (!shell || !variable)
		return ;
	// Check if the variable exists in the environment
	// for (size_t i = 0; shell->env[i]; i++)
    i = 0;
    while (shell->env[i])
	{
		if (strstr(shell->env[i], variable) == shell->env[i])
		{
			// Variable found, update its value
			var_len = ft_strlen(variable);
			equal_sign = ft_strchr(shell->env[i], '=');
			
			if (equal_sign && (equal_sign - shell->env[i] == (long)var_len))
			{
				value_len = ft_strlen(value);
				entry_len = var_len + value_len + 2;
				entry = (char *)malloc(sizeof(char) * entry_len);
				if (!entry)
					return ;
				strncpy(entry, variable, var_len);
				entry[var_len] = '=';
				strncpy(entry + var_len + 1, value, value_len);
				entry[entry_len - 1] = '\0';	
				// free(shell->env[i]);
				shell->env[i] = entry;
			}
			return ;
		}
        i++;
	}
	// Variable not found, add it to the environment
	var_len = ft_strlen(variable);
	value_len = ft_strlen(value);
	entry_len = var_len + value_len + 2;
	entry = (char *)malloc(sizeof(char) * entry_len);
	if (!entry)
		return;
	strncpy(entry, variable, var_len);
	entry[var_len] = '=';
	strncpy(entry + var_len + 1, value, value_len);
	entry[entry_len - 1] = '\0';
	shell->env = extend_env(shell->env, entry);
}

// int main(void)
// {
// 	t_shell shell;
// 	shell.env = (char **)malloc(sizeof(char *) * 3);
// 	shell.env[0] = strdup("VAR1=value1");
// 	shell.env[1] = strdup("VAR2=value2");
// 	shell.env[2] = NULL;

// 	char variable[] = "VAR1";
// 	char value[] = "value3";

// 	printf("Before:\n");
// 	for (size_t i = 0; shell.env[i]; i++)
// 		printf("%s\n", shell.env[i]);

// 	add_variable_to_environment(&shell, variable, value);

// 	printf("After:\n");
// 	for (size_t i = 0; shell.env[i]; i++)
// 		printf("%s\n", shell.env[i]);

// 	// Clean up the environment
// 	for (size_t i = 0; shell.env[i]; i++)
// 		free(shell.env[i]);
// 	free(shell.env);

// 	return 0;
// }
