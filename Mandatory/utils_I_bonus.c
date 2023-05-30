/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_I_bonus.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hahadiou <hahadiou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/05 17:51:43 by ael-khel          #+#    #+#             */
/*   Updated: 2023/05/30 00:03:17 by hahadiou         ###   ########.fr       */
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
		ft_dprintf(STDERR, "pipex: %s: %s\n", pathname, strerror(code));
		exit(EXIT_FAILURE);
	}
	return (fd);
}

void	ft_pipe(t_pipex *pipex)
{
	int	code;

	if (pipe(pipex->pipefd) < 0)
	{
		code = errno;
		ft_dprintf(STDERR, "pipex: pipe(): %s\n", strerror(code));
		exit(EXIT_FAILURE);
	}
}

pid_t	ft_fork(t_pipex *pipex)
{
	pid_t	pid;
	int		code;

	pid = fork();
	if (pid < 0)
	{
		code = errno;
		close(pipex->pipefd[0]);
		close(pipex->pipefd[1]);
		ft_dprintf(STDERR, "pipex: fork(): %s\n", strerror(code));
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

void	ft_execve(t_pipex *pipex)
{
	int	code;

	ft_clear(pipex->path);
	if (!pipex->path_cmd)
		pipex->path_cmd = ft_strdup(pipex->cmd[0]);
	if (execve(pipex->path_cmd, pipex->cmd, pipex->env) < 0)
	{
		code = errno;
		ft_dprintf(STDERR, "pipex: %s: %s\n", pipex->cmd[0], strerror(code));
		ft_clear(pipex->cmd);
		free(pipex->path_cmd);
		if (code == EACCES)
			exit(PERM_DENIED);
		exit(EXIT_FAILURE);
	}
}
