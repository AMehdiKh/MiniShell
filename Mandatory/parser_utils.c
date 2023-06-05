/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hahadiou <hahadiou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/04 15:31:11 by hahadiou          #+#    #+#             */
/*   Updated: 2023/06/04 17:30:01 by hahadiou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_check_pipe(t_lexer *lexer)
{
	while (lexer)
	{
		if (lexer->type == PIPE)
			return (1);
		lexer = lexer->next;
	}
	return (0);
}

int	ft_check_builtin(t_lexer *lexer)
{
	while (lexer)
	{
		if (lexer->type == BUILTIN)
			return (1);
		lexer = lexer->next;
	}
	return (0);
}

void	ft_save_std(int std[2])
{
	std[0] = dup(0);
	std[1] = dup(1);
}
