/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hahadiou <hahadiou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/09 19:32:45 by hahadiou          #+#    #+#             */
/*   Updated: 2023/05/15 21:57:32 by hahadiou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_newnode(t_lexer **node)
{
	t_lexer	*new;

	new = (t_lexer *)malloc(sizeof(t_lexer));
	if (!new)
	{
		exit(0); // to be changed
	}
	new->word = NULL;
	new->next = NULL;
	if (!*node)
	{
		*node = new;
		new->prev = NULL;
	}
	else
	{
		(*node)->next = new;
		new->prev = *node;
	}
}
