/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_get_env.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hahadiou <hahadiou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/15 23:11:11 by hahadiou          #+#    #+#             */
/*   Updated: 2023/05/28 17:43:08 by hahadiou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "libft.h"

char *ft_getenv(char *var, char **envp)
{
    size_t var_len = strlen(var);
    size_t i = 0;

    while (envp && envp[i])
    {
        const char *env_var = envp[i];
        char *eq_pos = strchr(env_var, '=');
        if (eq_pos && (size_t)(eq_pos - env_var) == var_len && memcmp(var, env_var, var_len) == 0)
            return strdup(eq_pos + 1);
        i++;
    }
    return NULL;
}
