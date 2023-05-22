
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>

enum e_token {
    BUILTIN,
    CMD,
    PIPE,
    W_A_FILE,
    W_T_FILE,
    R_FILE,
    HEREDOC,
    STDIN,
    STDOUT
};

typedef struct s_cmd {
    enum e_token type;  // CMD, BUILTIN, PIPE
    enum e_token lr_op; // </<<
    enum e_token rr_op; // >/>>
    char *lr;
    char *rr;
    char *cmd;
    char **argv;
    struct s_cmd *next;
} t_cmd;

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
        if (dup2(lr_fd, stdin_fd) == -1) {
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
        if (is_first_cmd) {
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
        if (dup2(rr_fd, stdout_fd) == -1) {
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
        if (!is_last_cmd) {
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

void execute_commands(t_cmd *head) {
    t_cmd *current_cmd = head;
    pid_t pid;
    int is_first_cmd = 1;
    int pipe_fd[2];

    while (current_cmd != NULL)
    {
        if (current_cmd->type == PIPE)
        {
            if (pipe(pipe_fd) == -1)
            {
                perror("Failed to create a pipe");
                exit(1);
                current_cmd = current_cmd->next;
            }
        }
        printf("cmd_curr: [%s]\n", current_cmd->cmd);
        printf("pipe[1]: [%i]\n", pipe_fd[1]);
        printf("pipe[0]: [%i]\n", pipe_fd[0]);
        pid = fork();
        if (pid == -1)
        {
            perror("Failed to fork");
            exit(1);
        }
        else if (pid == 0)
        {
            // Child process
            int is_last_cmd = (current_cmd->next == NULL);
            perform_redirection(current_cmd, is_first_cmd, is_last_cmd, pipe_fd);
        }
        else
        {
            // Parent process
            if (current_cmd->type == PIPE)
            {
                close(pipe_fd[0]);
                close(pipe_fd[1]);
            }

            int status;
            waitpid(pid, &status, 0);
            if (WIFEXITED(status))
            {
                printf("Child process exited with status: %d\n", WEXITSTATUS(status));
            }

            current_cmd = current_cmd->next;
            is_first_cmd = 0;
        }
    }
}

void free_commands(t_cmd *head) {
    t_cmd *current = head;
    while (current != NULL) {
        t_cmd *temp = current;
        current = current->next;
        free(temp->lr);
        free(temp->rr);
        free(temp->cmd);
        free(temp->argv);
        free(temp);
    }
}

int main() {
    // Create linked list of commands
    t_cmd *cmd1 = malloc(sizeof(t_cmd));
    cmd1->type = CMD;
    cmd1->lr_op = STDIN;
    cmd1->rr_op = PIPE;
    cmd1->lr = NULL;
    cmd1->rr = NULL;
    cmd1->cmd = "ls";
    cmd1->argv = malloc(3 * sizeof(char*));
    cmd1->argv[0] = "ls";
    cmd1->argv[1] = "-l";
    cmd1->argv[2] = NULL;
    
    
    t_cmd *pipe1 = malloc(sizeof(t_cmd));
    cmd1->type = PIPE;
    cmd1->lr_op = -1;
    cmd1->rr_op = -1;
    cmd1->lr = NULL;
    cmd1->rr = NULL;
    cmd1->cmd = NULL;
    cmd1->argv = NULL;


    t_cmd *cmd2 = malloc(sizeof(t_cmd));
    cmd2->type = CMD;
    cmd2->lr_op = PIPE;
    cmd2->rr_op = STDOUT;
    cmd2->lr = NULL;
    cmd2->rr = NULL;
    cmd2->cmd = "sort";
    cmd2->argv = malloc(3 * sizeof(char*));
    cmd2->argv[0] = "sort";
    cmd2->argv[1] = "-u";
    cmd2->argv[2] = NULL;

    t_cmd *pipe2 = malloc(sizeof(t_cmd));
    cmd1->type = PIPE;
    cmd1->lr_op = -1;
    cmd1->rr_op = -1;
    cmd1->lr = NULL;
    cmd1->rr = NULL;
    cmd1->cmd = NULL;
    cmd1->argv = NULL;

    t_cmd *cmd3 = malloc(sizeof(t_cmd));
    cmd3->type = CMD;
    cmd3->lr_op = PIPE;
    cmd3->rr_op = STDOUT;
    cmd3->lr = NULL;
    cmd3->rr = NULL;
    cmd3->cmd = "wc";
    cmd3->argv = malloc(3 * sizeof(char*));
    cmd3->argv[0] = "wc";
    cmd3->argv[1] = "-l";
    cmd3->argv[2] = NULL;

    cmd1->next = pipe1;
    pipe1->next = cmd2;
    cmd2->next = pipe2;
    pipe2->next = cmd3;
    cmd3->next = NULL;

    // Execute the commands
    execute_commands(cmd1);

    // Free the linked list
    // free_commands(cmd1);

    return 0;
}

/*
The error message "Failed to redirect stdout to the pipe" will be printed when there is an issue with redirecting the standard output (stdout) of a command to the pipe.

Here are some scenarios where this error message may occur:

    The pipe file descriptor is not valid or not open for writing.
    The dup2() function fails to redirect the stdout file descriptor to the pipe file descriptor.
    The pipe has been closed prematurely or is not properly set up before attempting redirection.

Please note that without the actual code implementation, it is difficult to pinpoint the exact cause of the error. The error message itself indicates a problem with redirecting stdout to the pipe, and you would need to debug the code and examine the specific context to identify the root cause of the issue.
*/