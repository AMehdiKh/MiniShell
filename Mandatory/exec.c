/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hahadiou <hahadiou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/24 23:02:44 by ael-khel          #+#    #+#             */
/*   Updated: 2023/05/29 23:26:57 by hahadiou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
void	exec_cmd(t_lexer *curr, t_shell *shell);

void	ls(t_lexer *node, t_shell *shell, int pfds[2])
{
	close(STDOUT_FILENO);
	dup(pfds[1]);
	close(pfds[0]);
	close(pfds[1]);
	exec_cmd(node, shell);
}

void	rs(t_lexer *node, t_shell *shell, int pfds[2])
{
	close(STDIN_FILENO);
	dup(pfds[0]);
	close(pfds[0]);
	close(pfds[1]);
	exec_cmd(node, shell);
}

void	exec_pipe(t_lexer *node, t_shell *shell)
{
	pid_t	cps;
	int		pfds[2];
	int		status;

	node->type = CMD;
	pipe(pfds);
	cps = fork();
	if (!cps)
		ls(node, shell, pfds);
	rs(node->next, shell, pfds);
	close(pfds[0]);
	close(pfds[1]);
	waitpid(cps, &status, 0);
	shell->exit_status = status >> 8;
}
/*
	THE CODE BELOW FOR SETUP AND EXECUTE PIPES
*/

char	*get_cmd(char **paths, char *cmd)
{
	// to add open dir to check if there is a / in the begin of command if it is a directoty or abs path
	char	*tmp;
	char	*command;

	if (cmd[0] == '/')
	{
		cmd = ft_strchr(cmd, '/');
		if (ft_strrchr(cmd, '/') == NULL)
			return (0);
	}
	while (*paths)
	{
		tmp = ft_strjoin(*paths, "/", 0);
		command = ft_strjoin(tmp, cmd, 1);
		if (access(command, F_OK | X_OK) == 0)
			return (command);
		free(command);
		paths++;
	}
	return (NULL);
}

void	exec_env_cmd(t_lexer *node, t_shell *data)
{
	char	**argv;
	char	*cmd_path;
	char	**paths;

	argv = ft_split(node->word, ' ', 0);
	cmd_path = argv[0];
	paths = ft_split(ft_getenv("PATH", data->env), ':', 0);
	cmd_path = get_cmd(paths, argv[0]);
	if (cmd_path == NULL)
	{
		ft_dprintf(2, "minishell: %s: command not found\n", argv[0]);
		data->exit_status = 127;
		exit (data->exit_status);
	}
	execve(cmd_path, argv, data->env);
}

void	executables(t_lexer *node, t_shell *shell)
{
	// if (builtin(node, shell))
	// 	return ;
	exec_env_cmd(node, shell);
}

/*
	THE CODE BELOW EXECUTE THE ENV_CMDS
*/

void	setup_input(t_lexer *node, t_shell *data)
{
	printf("setup_in\n");
	int		in_file;

	if (node->word)
	{
		while (node->type == R_FILE)
			node = node->next;
		if (access(node->word, F_OK) == 0)
		{
			in_file = open(node->word, O_RDONLY);
			dup2(in_file, STDIN_FILENO);
		}
		else
		{
			ft_dprintf(2, "minishell: %s: No such file or directory\n", node->word);
			data->exit_status = 2;
			exit (EXIT_FAILURE);
		}
	}
}

void	setup_heredoc(t_lexer *node)
{
	char	*line;
	int		pfds[2];

	pipe(pfds);
	while (-42)
	{
		line = readline("> ");
		if (ft_strnstr(line, node->word, ft_strlen(node->word)))
			break ;
		ft_dprintf(pfds[1], "%s\n", line);
	}
	close(pfds[1]);
	dup2(pfds[0], STDIN_FILENO);
	close(pfds[0]);
	free(line);
}

void	setup_output(t_lexer *node)
{
	close(STDERR_FILENO);
	while (node->type == W_A_FILE || node->type == W_T_FILE)
	{
		if (node->type == W_T_FILE)
			open(node->word, O_WRONLY | O_TRUNC | O_CREAT, 0644);
		else if (node->type == W_A_FILE)
			open(node->word, O_WRONLY | O_APPEND | O_CREAT, 0644);
		node = node->next;
		close(1);
	}
	if (node->type == W_T_FILE)
		open(node->word, O_WRONLY | O_TRUNC | O_CREAT, 0644);
	else if (node->type == W_A_FILE)
		open(node->word, O_WRONLY | O_APPEND | O_CREAT, 0644);
}

void	exec_redirects(t_lexer *node, t_shell *data)
{
	t_lexer	*tmp;

	tmp = node;
	if (node->type == R_FILE)
		setup_input(node, data);
	else if (node->type == HEREDOC)
		setup_heredoc(node);
	else
		setup_output(node);
	tmp->type = CMD;
	while ((node->type != CMD || node->type != BUILTIN ) && node->type != PIPE)
		node = node->next;
	if (node->type == CMD || node->type == BUILTIN)
		exec_cmd(tmp, data);
	else
		exec_pipe(node, data);
}

void	exec_cmd(t_lexer *curr, t_shell *shell)
{
	if (curr->type == PIPE)
		exec_pipe(curr, shell);
	else if (curr->type == CMD)
		executables(curr, shell);
	else
		exec_redirects(curr, shell);
	exit (shell->exit_status);
}

void	exec(t_lexer *lexer, t_shell *shell)
{
	t_lexer	*head = shell->lexer;
	t_lexer	*tmp;
	
	while (head != NULL)
	{
		if (head->type == PIPE && (head->prev->type == CMD || head->prev->type == BUILTIN))
		{
			tmp = head->prev;
			tmp->type = PIPE;
			tmp->next = head->next;
			free(head);
			head = tmp->next;
		}
		else
			head = head->next;
	}
	printLexer(shell->lexer);
	int	status;
	if (fork() == 0)
		exec_cmd(lexer, shell);
	waitpid(-1, &status, 0);
}
