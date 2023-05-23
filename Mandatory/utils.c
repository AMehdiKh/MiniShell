/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hahadiou <hahadiou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/09 19:32:45 by hahadiou          #+#    #+#             */
/*   Updated: 2023/05/23 22:19:00 by hahadiou         ###   ########.fr       */
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

t_cmd	*ft_newnode_cmd(t_token type, t_data *data)
{
	t_cmd	*new;

	new = (t_cmd *)malloc(sizeof(t_cmd));
	if (!new)
		return (NULL);
	new->cmd = data->abs_cmd;
    new->argv = data->argv;
    new->type = type;
    new->lr_op = data->lr_op;
    new->rr_op = data->rr_op;
    new->lr = data->lr;
    new->rr = data->rr;
	new->next = NULL;
	return (new);
}

t_cmd	*ft_newnode_pipe(t_token type)
{
	t_cmd	*new;

	new = (t_cmd *)malloc(sizeof(t_cmd));
	if (!new)
		return (NULL);
	new->cmd = NULL;
    new->argv = NULL;
    new->type = type;
    new->lr_op = -1;
    new->rr_op = -1;
    new->lr = NULL;
    new->rr = NULL;
	new->next = NULL;
	return (new);
}

void ft_cmdadd_back(t_cmd **lst, t_cmd *new)
{
    t_cmd *last = *lst;
    
    if (last)
    {
        while (last->next)
            last = last->next;
        last->next = new;
    }
    else
    {
        *lst = new;
    }
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

void print_cmd_node(t_cmd *head)
{
    t_cmd *current = head;
    printf("                   PARSER                         \n");
    while (current != NULL)
    {
        if (current->cmd)
            printf("cmd: %s\n", current->cmd);

        if (current->argv)
        {
            printf("argv: ");
            for (int i = 0 ; current->argv[i]; i++)
                printf("%s ", current->argv[i]);
            printf("\n");
        }
        printf("Type: ");
        if (current->type)
        {
            mini_print_token(current->type);
            printf("\n");
        }
        printf("lr: %s\n", current->lr);
        printf("rr: %s\n", current->rr);
        printf("lr_op: ");
        if (current->lr_op == STDIN)
            printf("STDIN");
        else
            print_token(current->lr_op);
        printf("\n");
        printf("rr_op: ");
        print_token(current->rr_op);
        printf("\n");
        printf("\n");
        current = current->next;
    }
}

void	ft_cmd_clear(t_cmd **lst)
{
	t_cmd *current;
	t_cmd *next;

	if (!lst || !(*lst))
		return ;
	current = *lst;
	while (current)
	{
		next = current->next;
		if (current->cmd)
			free(current->cmd);
        // if (current->argv)
        // {
            // for (int i = 0; current->argv[i]; i++)
            //     free (current->argv[i]);
            // free(current->argv);
        // }
		free(current);
		current = next;
	}
	*lst = NULL;
}

char    *get_last_file_word(t_lexer *head, t_token type[2])
{
    char *last_word;
    t_lexer *current;

    last_word = NULL;
    current = head;
    while (current != NULL)
    {
        // if (current->type == R_FILE || current->type == W_A_FILE || current->type == W_T_FILE)
        if (current->type == type[0] || current->type == type[1])
        {
            last_word = current->word;
        }
        current = current->next;
    }
    return (last_word);
}

void    heredoc(t_shell *shell, char *delimiter, bool expand)
{
	char	*line[2];
    int     fd;
	
    fd = open(delimiter, O_RDWR | O_CREAT | O_APPEND, 0644);
    while (1)
    {
        ft_dprintf(0, "> ");
        line[0] = get_next_line(0);
        if (!line[0] || ft_strnstr(line[0], delimiter, ft_strlen(delimiter)))
            break ;
        line[1] = expander(line[0], shell);
        if (line[1] && expand)
            ft_dprintf(fd, "%s", line[1]);
        else
            ft_dprintf(fd, "%s", line[0]);
    }
    close(fd);
}
