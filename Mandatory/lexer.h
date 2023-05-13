/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ael-khel <ael-khel@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/27 13:24:31 by ael-khel          #+#    #+#             */
/*   Updated: 2023/05/12 20:39:34 by ael-khel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LEXER_H
# define LEXER_H

typedef enum e_token
{
	BUILT,
	CMD,
	PIPE,
	W_A_FILE,
	W_T_FILE,
	R_FILE,
	HEREDOC,
}	t_token;

typedef struct s_lexer
{
	t_token			type;
	char			*word;
	struct s_lexer	*prev;
	struct s_lexer	*next;
}	t_lexer;

int	lexer(const char *input, int size, t_lexer *lexer, bool bonus);

#endif
