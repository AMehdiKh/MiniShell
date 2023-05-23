/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hahadiou <hahadiou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/24 23:02:44 by ael-khel          #+#    #+#             */
/*   Updated: 2023/05/23 23:04:00 by hahadiou         ###   ########.fr       */
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
		ft_cmdadd_back(&(shell->cmd), ft_newnode_cmd(lexer->type, data));
	}
	else if (type == BUILTIN)
	{
		data->argv = ft_split(lexer->word, ' ', 0);
		data->abs_cmd = ft_strdup(data->argv[0]);
		ft_cmdadd_back(&(shell->cmd), ft_newnode_cmd(lexer->type, data));
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
/********************************************************************/

t_cmd* create_cmd_node(t_token type, t_token lr_op, t_token rr_op, char* lr, char* rr, char* cmd, char** argv)
{
    t_cmd* node = (t_cmd*)malloc(sizeof(t_cmd));
    
    if (node == NULL)
        return NULL;

    node->type = type;
    node->lr_op = lr_op;
    node->rr_op = rr_op;
    node->lr = lr;
    node->rr = rr;
    node->cmd = cmd;
    node->argv = argv;
    node->next = NULL;

    return (node);
}

t_cmd* create_cmd_list(t_lexer* lexer, t_shell* shell)
{
    t_lexer* current = lexer;
    t_cmd* head = NULL;
    t_cmd* tail = NULL;
    t_token lr_op = STDIN;
    t_token rr_op = STDOUT;
    char* output_file = NULL;
    char* heredoc = NULL;
    char* lr = NULL;
    char* rr = NULL;

    while (current != NULL)
    {
        t_token type = current->type;
        char* word = current->word;
        if (current->prev != NULL && current->prev->type == PIPE)
            lr_op = PIPE;

        if (current->type == R_FILE)
        {
            // printf("in lr: %s\n", word);
            lr_op = current->type;
            lr = word;
        }
        else if (current->type == W_T_FILE)
        {
            // printf("in rr : %s\n", word);
            rr_op = current->type;
            rr = word;
        }
        else if (current->type == W_A_FILE)
        {
            rr_op = W_A_FILE;
            rr = word;
        }
        else if (current->type == HEREDOC)
        {
            lr_op = current->type;
            heredoc = word;
        }
        else if (current->type == CMD)
        {
            char** cmd_path = ft_split(ft_getenv("PATH", shell->env, -1), ':', 0);
            char** argv = ft_split(current->word, ' ', 0);
            char* cmd = get_cmd(cmd_path, argv[0]);
            argv[0] = cmd;

            // ...

            t_cmd* node = create_cmd_node(type, lr_op, rr_op, lr, rr, cmd, argv);
            if (node == NULL)
            {
                // Handle memory allocation failure
                // Cleanup any previously allocated memory if necessary
                return NULL;
            }

            // ...

            // Check if the input file is located to the right of the command
            if (lr_op == R_FILE)
            {
                node->lr = lr;
                lr = NULL;
            }
            else if (lr_op == HEREDOC)
            {
                node->lr = heredoc;
                heredoc = NULL;
            }
            else
                node->lr = NULL;

            // Check if the output file is located to the right of the command
            if (rr_op == W_T_FILE)
            {
                node->rr = output_file;
                output_file = NULL;
            }
            else if (rr_op == W_A_FILE)
            {
                node->rr = output_file;
                output_file = NULL;
            }
            else
                node->rr = NULL;

            // ...

            if (head == NULL) {
                head = node;
                tail = node;
            } else {
                tail->next = node;
                tail = node;
            }

            // Reset lr_op and rr_op for the next command
            lr_op = STDIN;
            rr_op = STDOUT;
        }
        current = current->next;
    }

    // Check if the last command has an output file
    if (output_file != NULL) {
        rr = output_file;
        t_cmd* node = create_cmd_node(CMD, lr_op, rr_op, NULL, rr, NULL, NULL);
        if (node == NULL) {
            // Handle memory allocation failure
            // Cleanup any previously allocated memory if necessary
            return NULL;
        }

        if (head == NULL) {
            head = node;
            tail = node;
        } else {
            tail->next = node;
            tail = node;
        }
    }

    return head;
}

void execute_commands(t_shell *shell, t_cmd* cmd_list)
{
    t_cmd* current = cmd_list;
    int prev_pipe[2] = { -1, -1 };
    int input_fd = STDIN_FILENO;

    while (current != NULL) {
        int curr_pipe[2] = { -1, -1 };

        // Create a new pipe for the current command
        if (current->next != NULL) {
            if (pipe(curr_pipe) == -1) {
                // Handle pipe creation failure
                // Cleanup any previously allocated resources if necessary
                return;
            }
        }

        pid_t pid = fork();
        if (pid == -1) {
            // Handle fork failure
            // Cleanup any previously allocated resources if necessary
            return ;
        }

        if (pid == 0) {
            // Child process

            // Set up input and output redirections
            if (current->lr_op == R_FILE)
            {
                int fd = open(current->lr, O_RDONLY);
                if (fd == -1) {
                    // Handle file open failure
                    // Cleanup any previously allocated resources if necessary
                    exit(1);
                }

                dup2(fd, STDIN_FILENO);
                close(fd);
            }
            else if (current->lr_op == HEREDOC)
            {
                heredoc(shell, current->lr, false);
                // char buf[4096];
                // read(hd, buf, 100);
                // printf("heredoc: %s\n", buf);
                // dup2(hd, STDIN_FILENO);
            }
            else if (current->lr_op == PIPE)
            {
                dup2(input_fd, STDIN_FILENO);
            }

            if (current->rr_op == W_T_FILE)
            {
                int fd = open(current->rr, O_WRONLY | O_CREAT | O_TRUNC, 0666);
                if (fd == -1)
                {
                    // Handle file open failure
                    // Cleanup any previously allocated resources if necessary
                    exit(1);
                }

                dup2(fd, STDOUT_FILENO);
                close(fd);
            }
            else if (current->rr_op == W_A_FILE)
            {
                int fd = open(current->rr, O_WRONLY | O_CREAT | O_APPEND, 0666);
                if (fd == -1)
                {
                    // Handle file open failure
                    // Cleanup any previously allocated resources if necessary
                    exit(1);
                }

                dup2(fd, STDOUT_FILENO);
                close(fd);
            }
            else if (current->rr_op == PIPE) {
                dup2(curr_pipe[1], STDOUT_FILENO);
            }

            // Close pipe file descriptors
            if (prev_pipe[0] != -1)
                close(prev_pipe[0]);
            if (prev_pipe[1] != -1)
                close(prev_pipe[1]);
            if (curr_pipe[0] != -1)
                close(curr_pipe[0]);
            if (curr_pipe[1] != -1)
                close(curr_pipe[1]);

            // Execute the command
            execve(current->cmd, current->argv, shell->env);
            // execvp(current->cmd, current->argv);

            // Handle execvp failure
            exit(1);
        } else {
            // Parent process

            // Close pipe file descriptors that are not needed anymore
            if (prev_pipe[0] != -1)
                close(prev_pipe[0]);
            if (prev_pipe[1] != -1)
                close(prev_pipe[1]);

            // Close input file descriptor if it's not STDIN
            if (input_fd != STDIN_FILENO)
                close(input_fd);

            // Store the current pipe as the previous pipe for the next iteration
            prev_pipe[0] = curr_pipe[0];
            prev_pipe[1] = curr_pipe[1];

            // Store the read end of the current pipe as the input for the next command
            input_fd = curr_pipe[0];

            // Move to the next command
            current = current->next;
        }
    }

    // Close the last remaining pipe file descriptors
    if (prev_pipe[0] != -1)
        close(prev_pipe[0]);
    if (prev_pipe[1] != -1)
        close(prev_pipe[1]);

    // Close input file descriptor if it's not STDIN
    if (input_fd != STDIN_FILENO)
        close(input_fd);

    // Wait for all child processes to complete
    while (wait(NULL) > 0)
        continue;
}


void    parser(t_shell *shell)
{
	// printLexer(shell->lexer);
	t_lexer *head;
	// t_cmd	*head_cmd;

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
    // print_cmd_node(shell->cmd);
	execute_commands(shell, shell->cmd);
	// print_cmd_node(shell->cmd);
	ft_cmd_clear(&(shell->cmd));
}
