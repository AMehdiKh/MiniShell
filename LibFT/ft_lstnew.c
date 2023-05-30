/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstnew.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ael-khel <ael-khel@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/05 12:19:15 by Ael-khel          #+#    #+#             */
/*   Updated: 2023/05/30 03:24:38 by ael-khel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

t_list	*ft_lstnew(void *content)
{
	t_list	*new;

	new = (t_list *)malloc(sizeof(t_list));
	if (!new)
		return (NULL);
	new->content = content;
	new->next = NULL;
	return (new);
}

void	ft_newnode(t_shell *shell)
{
	t_lexer	*new;
	t_lexer *node;

	new = (t_lexer *)malloc(sizeof(t_lexer));
	if (!new)
	{
		exit(0); // to be changed
	}
	new->word = NULL;
	new->next = NULL;
	node = ft_lexer_last(shell->lexer);
	if (!node)
	{
		shell->lexer = new;
		new->prev = NULL;
	}
	else
	{
		(node)->next = new;
		new->prev = node;
	}
}
