/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hahadiou <hahadiou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/24 23:02:44 by ael-khel          #+#    #+#             */
/*   Updated: 2023/05/24 20:28:15 by hahadiou         ###   ########.fr       */
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

void setup_lr_cmd(t_lexer *tail, t_data *data)
{
    t_lexer *curr = tail;
    data->lr_op = STDIN; // Set default lr_op to STDIN

    while (curr != NULL)
    {
        if (curr->type == R_FILE || curr->type == HEREDOC || curr->type == STDIN)
        {
            data->lr_op = curr->type;
            if (curr->type != STDIN)
                data->lr = curr->word;
            break;
        }
        if (curr->type == PIPE)
        {
            data->lr_op = PIPE;
            break;
        }
        curr = curr->prev;
    }
}

void setup_rr_cmd(t_lexer *head, t_data *data)
{
    t_lexer *curr = head;
    data->rr_op = STDOUT; // Set default rr_op to STDOUT

    while (curr != NULL)
    {
        if ((curr->type == R_FILE || curr->type == HEREDOC) && curr->type != PIPE)
        {
            if (curr->type == R_FILE)
                data->lr_op = R_FILE;
            else
                data->lr_op = HEREDOC;
            data->lr = curr->word;
        }
        if (curr->type == W_A_FILE || curr->type == W_T_FILE || curr->type == STDOUT)
        {
            data->rr_op = curr->type;
            if (curr->type != STDOUT)
                data->rr = curr->word;
            break;
        }
        if (curr->type == PIPE && data->rr_op != W_A_FILE && data->rr_op != W_T_FILE)
        {
            data->rr_op = PIPE;
            break ;
        }
        curr = curr->next;
    }
}

static	void	init_data(t_data *data)
{
	data->abs_cmd = NULL;
	data->argv = NULL;
	data->cmd_path = NULL;
	data->lr = NULL;
	data->rr = NULL;
	data->lr_op = STDIN;
	data->rr_op = STDOUT;
}

void setup_command(t_shell *shell, t_lexer *lexer, t_token type)
{
	t_data	data[1];

	init_data(data);
	setup_lr_cmd(lexer, data);
	setup_rr_cmd(lexer, data);
	if (type == CMD)
	{
		data->cmd_path = ft_split(ft_getenv("PATH", shell->env, -1), ':', 0);
		data->argv = ft_split(lexer->word, ' ', 0);
		data->abs_cmd = get_cmd(data->cmd_path, data->argv[0]);
		data->argv[0] = data->abs_cmd;
		ft_cmdadd_back(&(shell->cmd), ft_newnode_cmd(type, data));
	}
	else if (type == BUILTIN)
	{
		data->argv = ft_split(lexer->word, ' ', 0);
		data->abs_cmd = ft_strdup(data->argv[0]);
		ft_cmdadd_back(&(shell->cmd), ft_newnode_cmd(BUILTIN, data));
	}
}

void    parser(t_shell *shell)
{
	// printLexer(shell->lexer);
	t_lexer *head;
	t_cmd	*head_cmd;

	head = shell->lexer;
    // head_cmd = create_cmd_list(head, shell);
	// print_cmd_node(shell->cmd);
	while (head)
	{
		if (head->type == BUILTIN)
			setup_command(shell, head, BUILTIN);
		else if (head->type == CMD)
			setup_command(shell, head, CMD);
		head = head->next;
	}
    head_cmd = shell->cmd;
    // print_cmd_node(head_cmd);
	execute_commands(shell, shell->cmd);
	// print_cmd_node(shell->cmd);
	// ft_cmd_clear(&(shell->cmd));
}
