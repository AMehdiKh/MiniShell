/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_cmd_bonus.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hahadiou <hahadiou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/14 03:06:23 by ael-khel          #+#    #+#             */
/*   Updated: 2023/05/30 00:22:16 by hahadiou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_cmds_parse(char *arg, t_pipex *pipex)
{
	int	code;

	pipex->cmd = ft_split(arg, ' ', 0);
	if (!pipex->cmd)
	{
		code = errno;
		ft_dprintf(STDERR, "pipex: ft_split(): %s\n", strerror(code));
		exit(EXIT_FAILURE);
	}
}

void	ft_parse_path(t_pipex *pipex)
{
	int	code;
	int	i;

	i = 0;
	while (pipex->env && pipex->env[i])
	{
		if (!ft_strncmp("PATH=", pipex->env[i], 5))
		{
			pipex->path = ft_split(&pipex->env[i][5], ':', 0);
			if (!pipex->path)
			{
				code = errno;
				ft_clear(pipex->cmd);
				ft_dprintf(STDERR, "pipex: ft_split(): %s\n", strerror(code));
				exit(EXIT_FAILURE);
			}
			break ;
		}
		++i;
	}
	ft_slash_end(pipex);
}

void	ft_slash_end(t_pipex *pipex)
{
	char	*path;
	int		i;

	if (pipex->path)
	{
		i = 0;
		while (pipex->path[i])
		{
			path = ft_strjoin(pipex->path[i], "/", 0);
			free(pipex->path[i]);
			pipex->path[i] = path;
			++i;
		}
	}
}

void	ft_check_cmd(char *arg, t_pipex *pipex)
{
	int		i;

	ft_cmds_parse(arg, pipex);
	if (!access(pipex->cmd[0], F_OK))
		ft_execve(pipex);
	ft_parse_path(pipex);
	i = -1;
	while (pipex->path && pipex->path[++i])
	{
		pipex->path_cmd = ft_strjoin(pipex->path[i], pipex->cmd[0], 0);
		if (!access(pipex->path_cmd, F_OK))
			ft_execve(pipex);
		free(pipex->path_cmd);
	}
	if (pipex->path)
		ft_dprintf(STDERR, "pipex: %s: command not found\n", pipex->cmd[0]);
	else
		ft_dprintf(2, "pipex: %s: No such file or directory\n", pipex->cmd[0]);
	ft_clear(pipex->path);
	ft_clear(pipex->cmd);
	exit(CMD_NOT_FOUND);
}
