/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ael-khel <ael-khel@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/27 18:56:08 by ael-khel          #+#    #+#             */
/*   Updated: 2023/05/04 17:04:24 by ael-khel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "../LibFT/include/libft.h"
// # include "lexer.h"
# include <stdio.h>
# include <fcntl.h>
# include <readline/readline.h>
# include <readline/history.h>

typedef struct s_shell
{
	t_lexer	*lexer;
	t_list	*list;
	char	**env;
	char	*line;
	size_t	i;
	size_t	j;
	int		lexer_status;
	int		exit_status;
}	t_shell;

#endif
