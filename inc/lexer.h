/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hahadiou <hahadiou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/08 20:46:12 by hahadiou          #+#    #+#             */
/*   Updated: 2023/06/04 16:27:53 by hahadiou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LEXER_H
# define LEXER_H

# include <stdbool.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>

typedef enum e_token	t_token;
typedef struct s_lexer	t_lexer;

enum					e_token
{
	STDIN,
	STDOUT,
	STDERR,
	CMD,
	BUILTIN,
	PIPE,
	W_A_FILE,
	W_T_FILE,
	R_FILE,
	HEREDOC,
	PERM_DENIED = 126,
	CMD_NOT_FOUND = 127
};

struct					s_lexer
{
	t_token				type;
	char				*word;
	bool				expand;
	struct s_lexer		*prev;
	struct s_lexer		*next;
};

#endif
