/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_cmd.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hahadiou <hahadiou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/14 03:06:23 by ael-khel          #+#    #+#             */
/*   Updated: 2023/05/30 18:53:38 by hahadiou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_cmds_parse(char *arg, t_parser *parser)
{
	int	code;

	parser->cmd = ft_split_cmd(arg);
	if (!parser->cmd)
	{
		code = errno;
		ft_dprintf(STDERR, "minishell: ft_split(): %s\n", strerror(code));
		exit(EXIT_FAILURE);
	}
}

void	ft_parse_path(t_parser *parser)
{
	int	code;
	int	i;

	i = 0;
	while (parser->env && parser->env[i])
	{
		if (!ft_strncmp("PATH=", parser->env[i], 5))
		{
			parser->path = ft_split(&parser->env[i][5], ':', 0);
			if (!parser->path)
			{
				code = errno;
				ft_clear(parser->cmd);
				ft_dprintf(STDERR, "minishell: ft_split(): %s\n",
					strerror(code));
				exit(EXIT_FAILURE);
			}
			break ;
		}
		++i;
	}
	ft_slash_end(parser);
}

void	ft_slash_end(t_parser *parser)
{
	char	*path;
	int		i;

	if (parser->path)
	{
		i = 0;
		while (parser->path[i])
		{
			path = ft_strjoin(parser->path[i], "/", 0);
			free(parser->path[i]);
			parser->path[i] = path;
			++i;
		}
	}
}

void	ft_check_cmd(char *arg, t_parser *parser)
{
	int	i;

	ft_cmds_parse(arg, parser);
	if (!access(parser->cmd[0], F_OK))
		ft_execve(parser);
	ft_parse_path(parser);
	i = -1;
	while (parser->path && parser->path[++i])
	{
		parser->path_cmd = ft_strjoin(parser->path[i], parser->cmd[0], 0);
		if (!access(parser->path_cmd, F_OK))
			ft_execve(parser);
		free(parser->path_cmd);
	}
	if (parser->path)
		ft_dprintf(STDERR, "minishell: %s: command not found\n",
			parser->cmd[0]);
	else
		ft_dprintf(2, "minishell: %s: No such file or directory\n",
			parser->cmd[0]);
	ft_clear(parser->path);
	ft_clear(parser->cmd);
	exit(CMD_NOT_FOUND);
}
