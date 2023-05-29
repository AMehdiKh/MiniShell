#include <fcntl.h>
#include "minishell.h"
// Function to retrieve the last read and write file descriptors
void get_last_io_files(t_cmd* cmd_list, int* last_read_fd, int* last_write_fd)
{
    t_cmd* current = cmd_list;
    while (current->next != NULL)
        current = current->next;

    if (current->lr_op == R_FILE)
    {
        *last_read_fd = open(current->lr, O_RDONLY);
    }
    else if (current->lr_op == HEREDOC)
    {
        // Handle heredoc
    }

    if (current->rr_op == W_T_FILE)
    {
        *last_write_fd = open(current->rr, O_WRONLY | O_CREAT | O_TRUNC, 0666);
    }
    else if (current->rr_op == W_A_FILE)
    {
        *last_write_fd = open(current->rr, O_WRONLY | O_CREAT | O_APPEND, 0666);
    }
}

void execute_commands(t_shell *shell, t_cmd* cmd_list)
{
    t_cmd* current = cmd_list;
    int prev_pipe[2] = { -1, -1 };
    int input_fd = STDIN_FILENO;
    int last_read_fd = -1;
    int last_write_fd = -1;

    // Retrieve the last read and write file descriptors
    get_last_io_files(cmd_list, &last_read_fd, &last_write_fd);

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
                // Handle heredoc
            }
            else if (current->lr_op == PIPE)
            {
                dup2(prev_pipe[0], STDIN_FILENO);
            }
            else if (current->lr_op == LAST_READ)
            {
                dup2(last_read_fd, STDIN_FILENO);
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
            else if (current->rr_op == LAST_WRITE)
            {
                dup2(last_write_fd, STDOUT_FILENO);
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
            if (current->type == BUILTIN) {
                exec_builtin(shell, current);
            } else {
                execve(current->cmd, current->argv, shell->env);
            }

            // Handle execve failure
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
