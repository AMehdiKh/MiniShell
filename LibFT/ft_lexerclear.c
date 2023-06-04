/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lexerclear.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hahadiou <hahadiou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/15 21:27:43 by hahadiou          #+#    #+#             */
/*   Updated: 2023/05/15 21:41:12 by hahadiou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_lexer_clear(t_lexer **lexer)
{
	t_lexer	*current;
	t_lexer	*next;

	if (!lexer || !(*lexer))
		return ;
	current = *lexer;
	while (current)
	{
		next = current->next;
		if (current->word)
			free(current->word);
		free(current);
		current = next;
	}
	*lexer = NULL;
}
