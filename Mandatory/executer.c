/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executer.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hahadiou <hahadiou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/27 19:37:33 by ael-khel          #+#    #+#             */
/*   Updated: 2023/05/22 22:22:29 by hahadiou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"


void perform_redirection(t_cmd *cmd_node, int is_first_cmd, int is_last_cmd, int pipe_fd[2])
{
    int stdin_fd = STDIN_FILENO;
    int stdout_fd = STDOUT_FILENO;

    // Left read redirection
    if (cmd_node->lr_op == R_FILE)
    {
        int lr_fd = open(cmd_node->lr, O_RDONLY);
        if (lr_fd == -1)
        {
            perror("Failed to open file for left read redirection");
            exit(1);
        }
        if (dup2(lr_fd, stdin_fd) == -1)
        {
            perror("Failed to redirect stdin");
            exit(1);
        }
        close(lr_fd);
    } 
    else if (cmd_node->lr_op == HEREDOC)
    {
        FILE *heredoc_file = tmpfile();
        if (heredoc_file == NULL)
        {
            perror("Failed to create temporary file for here-document");
            exit(1);
        }
        fputs(cmd_node->lr, heredoc_file);
        rewind(heredoc_file);
        int lr_fd = fileno(heredoc_file);
        if (dup2(lr_fd, stdin_fd) == -1)
		{
            perror("Failed to redirect stdin");
            exit(1);
        }
    }
    else if (cmd_node->lr_op == STDIN)
    {
        // Read from stdin, no redirection needed
    }
    else if (cmd_node->lr_op == PIPE)
    {
        if (is_first_cmd)
		{
            // Redirect input from pipe
            if (dup2(pipe_fd[0], stdin_fd) == -1)
            {
                perror("Failed to redirect stdin from the pipe");
                exit(1);
            }
            close(pipe_fd[0]);
            close(pipe_fd[1]);
        }
    }

    // Right write redirection
    if (cmd_node->rr_op == W_A_FILE)
    {
        int rr_fd = open(cmd_node->rr, O_WRONLY | O_CREAT | O_APPEND, 0644);
        if (rr_fd == -1) {
            perror("Failed to open file for append write redirection");
            exit(1);
        }
        if (dup2(rr_fd, stdout_fd) == -1)
        {
            perror("Failed to redirect stdout");
            exit(1);
        }
        close(rr_fd);
    }
    else if (cmd_node->rr_op == W_T_FILE)
    {
        int rr_fd = open(cmd_node->rr, O_WRONLY | O_CREAT | O_TRUNC, 0644);
        if (rr_fd == -1) {
            perror("Failed to open file for truncate write redirection");
            exit(1);
        }
        if (dup2(rr_fd, stdout_fd) == -1)
		{
            perror("Failed to redirect stdout");
            exit(1);
        }
        close(rr_fd);
    } 
    else if (cmd_node->rr_op == STDOUT)
    {
        // Write to stdout, no redirection needed
    }
    else if (cmd_node->rr_op == PIPE)
    {
        if (!is_last_cmd)
		{
            // Redirect output to pipe
            if (dup2(pipe_fd[1], stdout_fd) == -1)
            {
                perror("Failed to redirect stdout to the pipe");
                exit(1);
            }
            close(pipe_fd[0]);
            close(pipe_fd[1]);
        }
    }

    // Execute the command
    execvp(cmd_node->cmd, cmd_node->argv);
    perror("Command execution failed");
    exit(1);
}

void	executer(t_shell *shell, int io[2])
{
	(void)io;
	t_cmd	*curr;
	int		pipe_fd[2];
	pid_t	pid;
	int	is_first_cmd = true;
	(void)pid;
	(void)is_first_cmd;
	print_cmd_node(shell->cmd);

	curr = shell->cmd;
	while (curr)
	{
		if (curr->type == PIPE)
		{
			if (pipe(pipe_fd) < 0)
			{
				perror("Failed to create a pipe");
				return ;
			}
			curr = curr->next;
		}
		// printf("pipe[1]: [%i]\n", pipe_fd[1]);
        // printf("pipe[0]: [%i]\n", pipe_fd[0]);
		// pid = fork();
		// if (pid < 0)
		// {
		// 	perror("Failed to fork");
		// 	return ;
		// }
		// else if (pid == 0)
		// {
		// 	int is_last_cmd = (curr->next == NULL);
		// 	perform_redirection(curr, is_first_cmd, is_last_cmd, pipe_fd);
		// }
		// else
		// {
		// 	if (curr->type == PIPE)
		// 	{
		// 		close(pipe_fd[0]);
		// 		close(pipe_fd[1]);
		// 	}
		// 	int status;
		// 	waitpid(pid, &status, 0);
		// 	if (WIFEXITED(status))
        //     {
        //         printf("Child process exited with status: %d\n", WEXITSTATUS(status));
        //     }
			curr = curr->next;
		// 	is_first_cmd = 0;
		// }
		
	}
	// print_cmd_node(curr);
	/****************************************************************************************/
	// printf("lr: [%s]\n", get_last_file_word(shell->lexer, (t_token[]){R_FILE, HEREDOC}));
	// printf("rr: [%s]\n", get_last_file_word(shell->lexer, (t_token[]){W_A_FILE, W_T_FILE}));
	// ft_cmd_clear(&(shell->cmd));
}
