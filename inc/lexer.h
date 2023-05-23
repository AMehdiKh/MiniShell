/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hahadiou <hahadiou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/08 20:46:12 by hahadiou          #+#    #+#             */
/*   Updated: 2023/05/23 19:14:55 by hahadiou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LEXER_H
# define LEXER_H

# include <stdlib.h>
# include <string.h>
# include <stdio.h>
# include <stdbool.h>

typedef enum 	e_token t_token;
typedef struct 	s_lexer t_lexer;

enum e_token
{
	BUILTIN,
	CMD,
	PIPE,
	W_A_FILE,
	W_T_FILE,
	R_FILE,
	HEREDOC,
	STDIN,
	STDOUT
};

struct s_lexer
{
	t_token			type;
	char			*word;
	struct s_lexer	*prev;
	struct s_lexer	*next;
};
/*
i have a linked list type of this structure give me the best scheme to setup redirections  if there is no redirection that the command will read or write from it will take the default toke STDIN OR STDOUT
type : contain the type of current node must be one of the enum given 
word : contain the name of command if type is CMD OR W_A_FILE OR W_T_FILE OR HEREDOC otherwize it will be NULL
prev and next are pointer of linked list
*/
#endif
