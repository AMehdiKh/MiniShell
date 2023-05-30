/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ael-khel <ael-khel@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/09 19:32:45 by hahadiou          #+#    #+#             */
/*   Updated: 2023/05/30 02:33:47 by ael-khel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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

int lstlexer_size(t_lexer *lexer)
{
    t_lexer *head;
    int     size;

    head = lexer;
    size = 0;
    while (head != NULL)
    {
        ++size;
        head = head->next;
    }
    return (size);
}

void print_token(t_token token)
{
    switch (token)
	{
        case BUILTIN:
			printf("BUILTIN\t|\t");
            break;
        case CMD:
            printf("CMD\t|\t");;
            break;
        case PIPE:
            printf("PIPE\t|\t");;
            break;
        case W_A_FILE:
            printf("W_A_FILE\t|\t");
            break;
        case W_T_FILE:
            printf("W_T_FILE\t|\t");
            break;
        case R_FILE:
            printf("R_FILE\t|\t");
            break;
        case HEREDOC:
            printf("HEREDOC\t|\t");
            break;
        case STDIN:
            printf("STDIN\t|\t");
        case STDOUT:
            printf("STDOUT\t|\t");
        case STDERR:
            break ;
        case PERM_DENIED:
            break ;
        case CMD_NOT_FOUND:
            break ;
    }
}

void mini_print_token(t_token token)
{
    if (token == CMD)
        printf("CMD");
    else if (token == BUILTIN)
        printf("BUILTIN");
    else if (token == R_FILE)
        printf("R_FILE");
    else if (token == HEREDOC)
        printf("HEREDOC");
    else if (token == W_A_FILE)
        printf("W_A_FILE");
    else if (token == W_T_FILE)
        printf("W_T_FILE");
}

void printLexer(t_lexer* lexer)
{
    printf("                   LEXER                         \n");
	printf("-------------------------------------------------\n");
    while (lexer != NULL)
    {
		printf("Type: ");
		print_token(lexer->type);
        printf("Word: %s\t\t |\n", lexer->word);
        //printf("Previous: %p\n", (void*)lexer->prev);
        // printf("Next: %p\n\n", (void*)lexer->next);
        lexer = lexer->next;
    }
	printf("-------------------------------------------------\n");
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
