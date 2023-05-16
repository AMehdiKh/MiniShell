/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_get_env.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hahadiou <hahadiou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/15 23:11:11 by hahadiou          #+#    #+#             */
/*   Updated: 2023/05/16 18:23:24 by hahadiou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "libft.h"

char *ft_getenv(char *var, char **envp, int n)
{
    char    *env_var;
    char    *eq_pos;
    long  var_len;
    size_t  i;

    var_len = ft_strlen(var) * (n < 0) + n * (n > 0);
    i = 0;
    while (envp && envp[i])
    {
        env_var = envp[i];
        eq_pos = ft_strchr(env_var, '=');
        if (eq_pos && (eq_pos - env_var) == var_len && !ft_strncmp(var, env_var, var_len))
            return (ft_strdup(eq_pos + 1));
        i++;
    }
    return (NULL);
}
