/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ael-khel <ael-khel@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/27 13:24:31 by ael-khel          #+#    #+#             */
/*   Updated: 2023/04/30 03:48:10 by ael-khel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LEXER_H
# define LEXER_H

typedef struct s_lexer
{
	t_token			type;
	char			*word;
	struct s_lexer	*prev;
	struct s_lexer	*next;
}	t_lexer;

typedef enum e_token
{
	BUILT,
	CMD,
	SUB,
	AND,
	OR,
	W_A_FILE,
	W_T_FILE,
	R_FILE,
	HEREDOC,
}	t_token;


int	lexer(const char *input, int size, t_lexer *lexer, bool bonus);

#endif
