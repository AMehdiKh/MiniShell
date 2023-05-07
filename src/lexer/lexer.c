/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hahadiou <hahadiou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/09 19:32:45 by hahadiou          #+#    #+#             */
/*   Updated: 2023/04/14 01:30:20 by hahadiou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lexer.h"

void	init_token(t_token *token, int	size)
{
	token->data = (char *)calloc((size + 1) , sizeof(char));

	token->type = 0;
	token->next	= NULL;
}

int get_type(char c, bool bonus)
{
    if (c == '\'')
        return (QUOTE);
    else if (c == '\"')
        return (DQUOTE);
    else if (c == '|')
        return (PIPE);
    else if (c == '&')
        return (AMPERSAND);
    else if (c == ' ')
        return (SPACE);
    else if (c == ';')
        return (SEMICOLON);
    else if (c == '\\')
        return (BACK_SLASH);
    else if (c == '\t')
        return (TAB);
    else if (c == '\n')
        return (NEW_LINE);
    else if (c == '>' || c == '<')
		return ((REDIRECT_TO * (c == '>') + REDIRECT_IN * (c == '<')));
    else if (c == 0)
        return 0;
	else if ((c == 40 || c == 41) && bonus)
		return ((O_PAR * (c == 40) + C_PAR * (c == 41)));
    else 
        return (GENERAL);
}

int	lexer(const char *input, int size, t_lexer *lexer, bool bonus)
{
	//printf("input: %s\n", input);
	if (!lexer)
		return (-1);
	if (!size)
	{
		lexer->tokens_nb = 0;
		return (0);
	}

	lexer->token = (t_token *)malloc(sizeof(t_token));

	t_token	*token = lexer->token;
	init_token(token, size);
	
	//printf("token: %s\n", token->data);
	int i = 0, j = 0, state = GENERAL_STATE, type;
	char c;
	while (input[i])
	{
		c = input[i];
		
		type = get_type(c, bonus);

		if (state == GENERAL_STATE)
		{
			if (type == QUOTE)
			{
				state = QUOTE_STATE;
				token->data[j++] = QUOTE;
				token->type = TOKEN;
			}
			else if (type == DQUOTE)
			{
				state = DQUOTE_STATE;
				token->data[j++] = DQUOTE;
				token->type = TOKEN;
			}
			else if (type == BACK_SLASH)
			{
				token->data[j++] = input[++i];
				token->type = BACK_SLASH;
			}
			else if (type == GENERAL)
			{
				token->data[j++] = c;
				token->type = TOKEN;
			}
			else if (type == SPACE)
			{
				if (j > 0)
				{
					token->data[j] = 0;
					token->next = (t_token *)malloc(sizeof(t_token));
					token = token->next;
					init_token(token, size - i);
					j = 0;
				}
			}
			else if (type == SEMICOLON || type == REDIRECT_TO || type == REDIRECT_IN || type == AMPERSAND || type == PIPE || ((type == O_PAR || type == C_PAR) && bonus))
			{
				if (j > 0)
				{
					token->data[j] = 0;
					token->next = (t_token *)malloc(sizeof(t_token));
					token = token->next;
					init_token(token, size - i);
					j = 0;
				}
				if (((input[i + 1] == '|' || input[i + 1] == '&') && bonus) || input[i + 1] == '>' || input[i + 1] == '<')
				{
					token->data[0] = ('|' * (input[i + 1] == '|') + '&' * (input[i + 1] == '&') + '<' * (input[i + 1] == '<') + '>' * (input[i + 1] == '>'));
					token->data[1] = ('|' * (input[i + 1] == '|') + '&' * (input[i + 1] == '&') + '<' * (input[i + 1] == '<') + '>' * (input[i + 1] == '>'));
					token->type = (260 * (input[i + 1] == '|') + 261 * (input[i + 1] == '&') + 262 * (input[i + 1] == '<') + 263 * (input[i + 1] == '>'));
					i++;
				}
				else
				{
					token->data[0] = type;
					//printf("type in fun: %c\n", type);
					//printf("data in fun: %s\n", token->data);
					token->data[1] = 0;
					token->type = type;
				}
				token->next =  (t_token *)malloc(sizeof(t_token));
				token = token->next;
				init_token(token, size - i);
			}
		}
		else if (state == DQUOTE_STATE || state == QUOTE_STATE)
		{
			token->data[j++] = c;
			if (type == DQUOTE || type == QUOTE)
			{
				state = GENERAL_STATE;
			}
		}
		else if (type == 0)
		{
			if (j > 0)
			{
				token->data[j] = 0;
				j = 0;
			}
		}
		i++;
	}
	return (-42);
}

	// void	parse_syntax_error(t_token *token)
	// {
		
	// }

int main() 
{
    t_lexer lexeur;
    //char input[] = " ls -la | grep foo >> || (echo hello world) < in | > out | << awk '{printf }'  | ; && << end >> out";
	const char* buf = (char *)calloc(500, sizeof(char));
	while (-42)
	{
		buf = readline("\033[1;33m♛\033[0m \033[1;32mlexer-0.1$\033[0m \033[1;33m⚡\033[0m ");
		add_history(buf);
		if (!strncmp(buf, "exit", 4))
			exit (1);
		lexer(buf, strlen(buf), &lexeur, true);
		t_token *token = lexeur.token;
    	while (token)
		{
			// to create a function that parse syntax of input (parse_syntax_erro();)
				
			// if ((token->type == PIPE && token->next->type == PIPE))
			// {
			// 	dprintf(2, "-lexer: syntax error near unexpected token `%s'", token->data);
			// 	exit (258);
			// }
        	printf("Token: [%s]\tType: [%d]\n", token->data, token->type);
         	token = token->next;
    	}
		// printf("%s\n", buf);
	}
    // int ret = lexer(buf, sizeof(buf), &lexeur, false);
    // if (ret == -1) {
    //     printf("Lexer failed to build.\n");
    //     return (-1);
    // }

    // t_token *token = lexeur.token;
    // while (token)
	// {
    //     printf("Token: [%s]\tType: [%d]\n", token->data, token->type);
    //      token = token->next;
    // }
    return (0);
}
