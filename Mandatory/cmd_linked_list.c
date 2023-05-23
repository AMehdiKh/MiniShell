#include <stdlib.h>
#include <string.h>

typedef enum e_token t_token;

enum e_token
{
	STDIN,
	STDOUT,
	NONE,
	BUILTIN,
	CMD,
	PIPE,
	W_A_FILE,
	W_T_FILE,
	R_FILE,
	HEREDOC
};

typedef struct s_lexer t_lexer;

struct s_lexer
{
	t_token type;
	char* word;
	struct s_lexer* prev;
	struct s_lexer* next;
};

typedef struct s_cmd t_cmd;

struct s_cmd
{
	t_token type;
	t_token lr_op;
	t_token rr_op;
	char* lr;
	char* rr;
	char* cmd;
	char** argv;
	t_cmd* next;
};

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

	return node;
}

t_cmd* create_cmd_list(t_lexer* lexer)
{
	t_lexer* current = lexer;
	t_cmd* head = NULL;
	t_cmd* tail = NULL;
	t_token lr_op = STDIN;

	while (current != NULL)
	{
		t_token type = current->type;
		char* word = current->word;
		t_token rr_op = STDOUT;
		char* lr = NULL;
		char* rr = NULL;
		char* cmd = NULL;
		char** argv = NULL;

		if (current->prev != NULL && current->prev->type == PIPE)
			lr_op = PIPE;

		if (current->type == R_FILE || current->type == HEREDOC)
		{
			lr_op = current->type;
			lr = word;
		}
		else if (current->type == W_A_FILE || current->type == W_T_FILE)
		{
			rr_op = current->type;
			rr = word;
		}
		else if (current->type == CMD)
		{
			cmd = word;

			// Parse command arguments
			// Here, you can implement your logic to split the word into arguments
			// and populate the argv array accordingly.

			argv = (char**)malloc(sizeof(char*) * 1);
			if (argv == NULL)
			{
				// Handle memory allocation failure
				// Cleanup any previously allocated memory if necessary
				return NULL;
			}

			// Populate the argv array based on your logic
			// ...

			// Set argc and argv appropriately
			// ...

			t_cmd* node = create_cmd_node(type, lr_op, rr_op, lr, rr, cmd, argv);
			if (node == NULL)
			{
				// Handle memory allocation failure
				// Cleanup any previously allocated memory if necessary
				return NULL;
			}

			if (head == NULL)
			{
				head = node;
				tail = node;
			}
			else
			{
				tail->next = node;
				tail = node;
			}

			// Reset lr_op for the next command
			lr_op = STDIN;
		}

		current = current->next;
	}

	return head;
}
