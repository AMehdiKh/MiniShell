/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hahadiou <hahadiou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/08 20:46:12 by hahadiou          #+#    #+#             */
/*   Updated: 2023/04/12 01:26:34 by hahadiou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LEXER_H
# define LEXER_H

# include <stdlib.h>
# include <string.h>
# include <stdio.h>
# include <stdbool.h>
# include <readline/readline.h>
# include <readline/history.h>

enum	
{
	GENERAL		= -1,
	PIPE		= '|',
	AMPERSAND	= '&',
	QUOTE		= 39,
	DQUOTE		= 34,
	SEMICOLON	= ';',
	SPACE		= ' ',
	BACK_SLASH	= '\\',
	TAB			= '\t',
	NEW_LINE	= '\n',
	REDIRECT_TO	= '>',
	REDIRECT_IN	= '<',
	O_PAR 		= '(',
	C_PAR 		= ')',
	TOKEN		= -1,
};

enum
{
	DQUOTE_STATE,
	QUOTE_STATE,
	
	BACKSLASH_STATE,
	GENERAL_STATE,
};

typedef struct	s_token	t_token;
typedef struct	s_lexer	t_lexer;

struct	s_token
{
	char	*data;
	int		type;
	t_token	*next;
};

struct	s_lexer
{
	t_token	*token;
	int	tokens_nb;
};

int	lexer(const char *input, int size, t_lexer *lexer, bool bonus);

#endif
