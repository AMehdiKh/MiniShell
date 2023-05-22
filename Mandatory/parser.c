/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hahadiou <hahadiou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/24 23:02:44 by ael-khel          #+#    #+#             */
/*   Updated: 2023/05/22 23:21:56 by hahadiou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	check_cmd(char *cmd)
{
	// to add open dir to open directory if given an absolute path to check if it is an directoty or abs path
	if (access(cmd, F_OK) < 0)
	{
		ft_dprintf(2, "No such file or directory: %s\n", cmd);
		// exit(126);
	}
	if (access(cmd, X_OK) < 0)
	{
		ft_dprintf(2, "Permission denied: %s\n", cmd);
		// exit(126);
	}
	return (1);
}

char	*get_cmd(char **paths, char *cmd)
{
	// to add open dir to check if there is a / in the begin of command if it is a directoty or abs path
	char	*tmp;
	char	*command;

	if (cmd[0] == '.')
		if (check_cmd(cmd) == 1)
			return (cmd);
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
		if (access(command, F_OK) == 0)
			return (command);
		free(command);
		paths++;
	}
	return (NULL);
}

void setup_lr_cmd(t_lexer *lexer, t_data *data)
{
    t_lexer *current = lexer->prev;
    while (current)
	{
        if (current->type == R_FILE || current->type == HEREDOC)
		{
            data->lr_op = current->type;
            data->lr = current->word;
            break ;
        }
        current = current->prev;
    }
	if (data->lr_op == NONE)
        data->lr_op = STDIN;
}

void setup_rr_cmd(t_lexer *lexer, t_data *data)
{
    t_lexer *current = lexer->next;
    while (current)
	{
        if (current->type == W_A_FILE || current->type == W_T_FILE)
		{
            data->rr_op = current->type;
            data->rr = current->word;
            break ;
        }
        current = current->next;
    }
	if (data->rr_op == NONE)
        data->rr_op = STDOUT;
}

static	void	init_data(t_data *data)
{
	data->lr = NULL;
	data->rr = NULL;
	data->lr_op = NONE;
	data->rr_op = NONE;
}

void setup_command(t_shell *shell, t_lexer *lexer, t_token type)
{
    char	**cmd_path;
    char	*abs_cmd;
    char	**argv;
	t_data	data;

	init_data(&data);
	setup_lr_cmd(lexer, &data);
	setup_rr_cmd(lexer, &data);
	if (type == CMD)
	{
		cmd_path = ft_split(ft_getenv("PATH", shell->env, -1), ':', 0);
		argv = ft_split(lexer->word, ' ', 0);
		abs_cmd = get_cmd(cmd_path, argv[0]);
		argv[0] = abs_cmd;
		ft_cmdadd_back(&(shell->cmd), ft_newnode_cmd(abs_cmd, argv, lexer->type, &data));
	}
	else if (type == BUILTIN)
	{
		argv = ft_split(lexer->word, ' ', 0);
		ft_cmdadd_back(&(shell->cmd), ft_newnode_cmd(ft_strdup(argv[0]), argv + 1, lexer->type, &data));
	}
}

void	setup_pipe(t_shell *shell)
{
	ft_cmdadd_back(&(shell->cmd), ft_newnode_pipe(PIPE));
}

// setup_lr(t_shell *shell, t_lexer *lexer, t_token type)
// {
// 	if (type == R_FILE)
// 		ft_cmdadd_back(&(shell->cmd), ft_newnode_pipe(R_FILE, (char **){lexer->word, NULL}));
// 	else if (type == HEREDOC)
// 		ft_cmdadd_back(&(shell->cmd), ft_newnode_pipe(HEREDOC, (char **){lexer->word, NULL}));
// 	else if (type == W_T_FILE)
// 		ft_cmdadd_back(&(shell->cmd), ft_newnode_pipe(W_T_FILE, (char **){NULL, lexer->word}));
// 	else if (type == W_A_FILE)
// 		ft_cmdadd_back(&(shell->cmd), ft_newnode_pipe(W_A_FILE, (char **){NULL, lexer->word}));
// }

void    parser(t_shell *shell)
{
	// printLexer(shell->lexer);
	t_lexer *head;

	head = shell->lexer;
	while (head)
	{
		if (head->type == PIPE)
			setup_pipe(shell);
		if (head->type == BUILTIN)
			setup_command(shell, head, BUILTIN);
		else if (head->type == CMD)
			setup_command(shell, head, CMD);
		// else if (head->type == R_FILE)
		// 	setup_lr(shell, head, R_FILE);
		// else if (head->type == HEREDOC)
		// 	setup_lr(shell, head, HEREDOC);
		// else if (head->type == W_A_FILE)
		// 	setup_lr(shell, head, W_A_FILE);
		// else if (head->type == W_T_FILE)
		// 	setup_lr(shell, head, W_T_FILE);
		head = head->next;
	}
	print_cmd_node(shell->cmd);
	ft_cmd_clear(&(shell->cmd));
}
