/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   new_pipe.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hahadiou <hahadiou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/28 22:11:09 by hahadiou          #+#    #+#             */
/*   Updated: 2023/05/28 22:12:28 by hahadiou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	left_side(t_lexer *nd, t_shell *data, int pdes[2])
{
	close(STDOUT_FILENO);
	dup(pdes[1]);
	close(pdes[0]);
	close(pdes[1]);
	exec_cmd(nd, data);
}

static void	right_side(t_lexer *nd, t_shell *data, int pdes[2])
{
	close(STDIN_FILENO);
	dup(pdes[0]);
	close(pdes[0]);
	close(pdes[1]);
	exec_cmd(nd, data);
}

//	bytes written on pipedes[1] can be read on pipedes[0]
void	exec_pipe(t_lexer *node, t_shell *data)
{
	pid_t	child_pid;
	int		pipedes[2];
	int		temp_status;

	node->type = CMD;
	// if (pipe(pipedes) == -1)
	// 	panic(data, PIPE_ERR, EXIT_FAILURE);
    pipe(pipedes);
	child_pid = fork();
	// if (child_pid == -1)
	// 	panic(data, FORK_ERR, EXIT_FAILURE);
	if (child_pid == 0)
		left_side(node, data, pipedes);
	right_side(node->next, data, pipedes);
	close(pipedes[0]);
	close(pipedes[1]);
	waitpid(child_pid, &temp_status, 0);
	data->exit_status = temp_status >> 8;
}
