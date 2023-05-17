/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hahadiou <hahadiou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/09 19:32:45 by hahadiou          #+#    #+#             */
/*   Updated: 2023/05/17 14:27:32 by hahadiou         ###   ########.fr       */
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

void printLexer(t_lexer* lexer)
{
    while (lexer != NULL)
    {
        printf("Type: %d\n", lexer->type);
        printf("Word: %s\n", lexer->word);
        //printf("Previous: %p\n", (void*)lexer->prev);
        // printf("Next: %p\n\n", (void*)lexer->next);
        lexer = lexer->next;
    }
}

void printList(t_list* list)
{
    while (list != NULL)
    {
        printf("Content: %s\n", list->content);
        // printf("Next: %p\n\n", (void*)list->next);
        list = list->next;
    }
}
