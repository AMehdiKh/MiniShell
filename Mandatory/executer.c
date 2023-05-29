/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executer.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hahadiou <hahadiou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/27 19:37:33 by ael-khel          #+#    #+#             */
/*   Updated: 2023/05/25 19:56:24 by hahadiou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"


void    exec_builtin_parent(t_shell *shell, t_cmd *cn)
{
    if (!ft_strncmp(cn->cmd, "cd", 2))
    {
        ft_cd_builtin(*(cn->argv + 1), shell);
    }
    else if (!ft_strncmp(cn->cmd, "export", 6))
    {
        ft_export_builtin(shell, cn->argv);
    }
    else if (!ft_strncmp(cn->cmd, "unset", 5))
    {
        ft_unset_builtin((cn->argv[1]), shell->env);
    }
}

void exec_builtin_child(t_shell *shell, t_cmd *cn)
{
    if (!ft_strncmp(cn->cmd, "echo", 4))
    {
        ft_echo_builtin(ft_count_strings(cn->argv), cn->argv, cn);
    }
    else if (!ft_strncmp(cn->cmd, "pwd", 3))
    {
        ft_pwd_builtin(shell);
    }
    // else if (!ft_strncmp(cn->cmd, "cd", 2))
    // {
    //     ft_cd_builtin(*(cn->argv + 1), shell);
    // }
    else if (!ft_strncmp(cn->cmd, "env", 3))
    {
        ft_env_builtin(cn->argv, shell);
    }
    // else if (!ft_strncmp(cn->cmd, "export", 6))
    // {
    //     ft_export_builtin(shell, cn->argv);
    // }
    // else if (!ft_strncmp(cn->cmd, "unset", 5))
    // {
    //     ft_unset_builtin((cn->argv[1]), shell->env);
    // }
}

void    execute_commands(t_shell *shell, t_cmd* cmd_list)
{
    t_cmd* current = cmd_list;
    int prev_pipe[2] = { -1, -1 };
    int input_fd = STDIN_FILENO;
    while (current != NULL)
    {
        int curr_pipe[2] = { -1, -1 };

        // Create a new pipe for the current command
        if (current->next != NULL)
        {
            if (pipe(curr_pipe) == -1)
            {
                // Handle pipe creation failure
                // Cleanup any previously allocated resources if necessary
                return ;
            }
        }
        pid_t pid = fork();
        if (pid == -1)
        {
            // Handle fork failure
            // Cleanup any previously allocated resources if necessary
            return ;
        }

        if (pid == 0)
        {
            // Child process

            // Set up input and output redirections
            if (current->lr_op == R_FILE)
            {
                int fd = open(current->lr, O_RDONLY);
                if (fd == -1)
                {
                    // Handle file open failure
                    // Cleanup any previously allocated resources if necessary
                    exit(1);
                }

                dup2(fd, STDIN_FILENO);
                close(fd);
            }
            else if (current->lr_op == HEREDOC)
            {
                int pipefd[2];
                if (pipe(pipefd) == -1)
                {
                    // Handle pipe creation failure
                    // Cleanup any previously allocated resources if necessary
                    return ;
                }
                pid_t heredoc_pid = fork();
                if (heredoc_pid == -1)
                {
                    // Handle fork failure
                    // Cleanup any previously allocated resources if necessary
                    return ;
                }

                if (heredoc_pid == 0)
                {
                    // Child process for heredoc

                    // Close the read end of the pipe
                    close(pipefd[0]);

                    // Write the heredoc content to the write end of the pipe
                    heredoc(shell, current->lr, 0, pipefd[1]);
                    // char* heredoc_content = "This is the heredoc content\n";
                    // write(pipefd[1], heredoc_content, strlen(heredoc_content));

                    // Close the write end of the pipe
                    close(pipefd[1]);

                    exit(0);
                }
                else
                {
                    // Parent process

                    // Close the write end of the pipe
                    close(pipefd[1]);

                    // Set up input redirection from the read end of the pipe
                    dup2(pipefd[0], STDIN_FILENO);

                    // Close the read end of the pipe
                    close(pipefd[0]);
                }
            }

            else if (current->lr_op == PIPE)
            {
                dup2(prev_pipe[0], STDIN_FILENO);
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
            else if (current->rr_op == PIPE)
            {
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
            if (current->type == BUILTIN && (!ft_strncmp(current->cmd, "echo", 4) \
                || !ft_strncmp(current->cmd, "pwd", 3) \
                || !ft_strncmp(current->cmd, "env", 3)))
            {
                exec_builtin_child(shell, current);
            }
            else
            {
                execve(current->cmd, current->argv, shell->env);
            }

            // Handle execve failure
            exit(1);
        }
        else
        {
            // Parent process
            if (current->type == BUILTIN)
                exec_builtin_parent(shell, current);
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
        continue ;
}

/**********************************************/
// void perform_redirection(t_cmd *cmd_node, int is_first_cmd, int is_last_cmd, int pipe_fd[2])
// {
//     int stdin_fd = STDIN_FILENO;
//     int stdout_fd = STDOUT_FILENO;

//     // Left read redirection
//     if (cmd_node->lr_op == R_FILE)
//     {
//         int lr_fd = open(cmd_node->lr, O_RDONLY);
//         if (lr_fd == -1)
//         {
//             perror("Failed to open file for left read redirection");
//             exit(1);
//         }
//         if (dup2(lr_fd, stdin_fd) == -1)
//         {
//             perror("Failed to redirect stdin");
//             exit(1);
//         }
//         close(lr_fd);
//     } 
//     else if (cmd_node->lr_op == HEREDOC)
//     {
//         FILE *heredoc_file = tmpfile();
//         if (heredoc_file == NULL)
//         {
//             perror("Failed to create temporary file for here-document");
//             exit(1);
//         }
//         fputs(cmd_node->lr, heredoc_file);
//         rewind(heredoc_file);
//         int lr_fd = fileno(heredoc_file);
//         if (dup2(lr_fd, stdin_fd) == -1)
// 		{
//             perror("Failed to redirect stdin");
//             exit(1);
//         }
//     }
//     else if (cmd_node->lr_op == STDIN)
//     {
//         // Read from stdin, no redirection needed
//     }
//     else if (cmd_node->lr_op == PIPE)
//     {
//         if (is_first_cmd)
// 		{
//             // Redirect input from pipe
//             if (dup2(pipe_fd[0], stdin_fd) == -1)
//             {
//                 perror("Failed to redirect stdin from the pipe");
//                 exit(1);
//             }
//             close(pipe_fd[0]);
//             close(pipe_fd[1]);
//         }
//     }

//     // Right write redirection
//     if (cmd_node->rr_op == W_A_FILE)
//     {
//         int rr_fd = open(cmd_node->rr, O_WRONLY | O_CREAT | O_APPEND, 0644);
//         if (rr_fd == -1) {
//             perror("Failed to open file for append write redirection");
//             exit(1);
//         }
//         if (dup2(rr_fd, stdout_fd) == -1)
//         {
//             perror("Failed to redirect stdout");
//             exit(1);
//         }
//         close(rr_fd);
//     }
//     else if (cmd_node->rr_op == W_T_FILE)
//     {
//         int rr_fd = open(cmd_node->rr, O_WRONLY | O_CREAT | O_TRUNC, 0644);
//         if (rr_fd == -1) {
//             perror("Failed to open file for truncate write redirection");
//             exit(1);
//         }
//         if (dup2(rr_fd, stdout_fd) == -1)
// 		{
//             perror("Failed to redirect stdout");
//             exit(1);
//         }
//         close(rr_fd);
//     } 
//     else if (cmd_node->rr_op == STDOUT)
//     {
//         // Write to stdout, no redirection needed
//     }
//     else if (cmd_node->rr_op == PIPE)
//     {
//         if (!is_last_cmd)
// 		{
//             // Redirect output to pipe
//             if (dup2(pipe_fd[1], stdout_fd) == -1)
//             {
//                 perror("Failed to redirect stdout to the pipe");
//                 exit(1);
//             }
//             close(pipe_fd[0]);
//             close(pipe_fd[1]);
//         }
//     }

//     // Execute the command
//     execvp(cmd_node->cmd, cmd_node->argv);
//     perror("Command execution failed");
//     exit(1);
// }

// void	executer(t_shell *shell, int io[2])
// {
// 	(void)io;
// 	t_cmd	*curr;
// 	int		pipe_fd[2];
// 	pid_t	pid;
// 	int	is_first_cmd = true;
// 	(void)pid;
// 	(void)is_first_cmd;
// 	print_cmd_node(shell->cmd);

// 	curr = shell->cmd;
// 	while (curr)
// 	{
// 		if (curr->type == PIPE)
// 		{
// 			if (pipe(pipe_fd) < 0)
// 			{
// 				perror("Failed to create a pipe");
// 				return ;
// 			}
// 			curr = curr->next;
// 		}
// 		// printf("pipe[1]: [%i]\n", pipe_fd[1]);
//         // printf("pipe[0]: [%i]\n", pipe_fd[0]);
// 		// pid = fork();
// 		// if (pid < 0)
// 		// {
// 		// 	perror("Failed to fork");
// 		// 	return ;
// 		// }
// 		// else if (pid == 0)
// 		// {
// 		// 	int is_last_cmd = (curr->next == NULL);
// 		// 	perform_redirection(curr, is_first_cmd, is_last_cmd, pipe_fd);
// 		// }
// 		// else
// 		// {
// 		// 	if (curr->type == PIPE)
// 		// 	{
// 		// 		close(pipe_fd[0]);
// 		// 		close(pipe_fd[1]);
// 		// 	}
// 		// 	int status;
// 		// 	waitpid(pid, &status, 0);
// 		// 	if (WIFEXITED(status))
//         //     {
//         //         printf("Child process exited with status: %d\n", WEXITSTATUS(status));
//         //     }
// 			curr = curr->next;
// 		// 	is_first_cmd = 0;
// 		// }
		
// 	}
// 	// print_cmd_node(curr);
// 	/****************************************************************************************/
// 	// printf("lr: [%s]\n", get_last_file_word(shell->lexer, (t_token[]){R_FILE, HEREDOC}));
// 	// printf("rr: [%s]\n", get_last_file_word(shell->lexer, (t_token[]){W_A_FILE, W_T_FILE}));
// 	// ft_cmd_clear(&(shell->cmd));
// }
