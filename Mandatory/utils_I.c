/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_I.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hahadiou <hahadiou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/05 17:51:43 by ael-khel          #+#    #+#             */
/*   Updated: 2023/05/30 18:53:38 by hahadiou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_open(const char *pathname, int flags, mode_t mode)
{
	int	fd;
	int	code;

	fd = open(pathname, flags, mode);
	if (fd < 0)
	{
		code = errno;
		ft_dprintf(STDERR, "minishell: %s: %s\n", pathname, strerror(code));
		exit(EXIT_FAILURE);
	}
	return (fd);
}

void	ft_pipe(t_parser *parser)
{
	int	code;

	if (pipe(parser->pipefd) < 0)
	{
		code = errno;
		ft_dprintf(STDERR, "minishell: pipe(): %s\n", strerror(code));
		exit(EXIT_FAILURE);
	}
}

pid_t	ft_fork(t_parser *parser)
{
	pid_t	pid;
	int		code;

	pid = fork();
	if (pid < 0)
	{
		code = errno;
		close(parser->pipefd[0]);
		close(parser->pipefd[1]);
		ft_dprintf(STDERR, "minishell: fork(): %s\n", strerror(code));
		exit(EXIT_FAILURE);
	}
	return (pid);
}

void	ft_dup2(int old, int new)
{
	if (old == new)
		return ;
	dup2(old, new);
	close(old);
}

void	ft_execve(t_parser *parser)
{
	int	code;

	ft_clear(parser->path);
	if (!parser->path_cmd)
		parser->path_cmd = ft_strdup(parser->cmd[0]);
	if (execve(parser->path_cmd, parser->cmd, parser->env) < 0)
	{
		code = errno;
		ft_dprintf(STDERR, "minishell: %s: %s\n", parser->cmd[0], strerror(code));
		ft_clear(parser->cmd);
		free(parser->path_cmd);
		if (code == EACCES)
			exit(PERM_DENIED);
		exit(EXIT_FAILURE);
	}
}
