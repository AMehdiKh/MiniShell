/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hahadiou <hahadiou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/27 19:37:33 by ael-khel          #+#    #+#             */
/*   Updated: 2023/05/30 18:17:27 by hahadiou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	child_signals(int signum)
{
	if (signum == SIGINT)
	{
		ft_dprintf(STDOUT_FILENO, "\n");
		shell->exit_status = 130;
		exit(130);
	}
}

void	dismiss_signal(int signum)
{
	if (signum == SIGINT)
	{
		ft_dprintf(STDOUT, "\n");
		rl_on_new_line();
		rl_replace_line("", 0);
		rl_redisplay();
		shell->exit_status = 130;
	}
}

void	config_signals(void)
{
	struct sigaction	sa;

	sa.sa_handler = &dismiss_signal;
	sa.sa_flags = SA_RESTART;
	sigemptyset(&sa.sa_mask);
	sigaddset(&sa.sa_mask, SIGINT);
	sigaction(SIGINT, &sa, NULL);
	signal(SIGQUIT, SIG_IGN);
}
