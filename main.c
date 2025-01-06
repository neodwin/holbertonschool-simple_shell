#include "shell.h"

/**
 * main - Entry point for the shell
 *
 * This function initializes the shell, handles user input, and
 * executes commands in a loop until the user exits.
 *
 * Return: 0 on success
 */
int main(void)
{
	char *input;

	while (1)
	{
		/* Check if input is from an interactive terminal */
		if (isatty(STDIN_FILENO))
			display_prompt();

		/* Read user input */
		input = get_input();
		if (input == NULL)
			break;

		/* Process and execute the command */
		execute_command(input);
		free(input);
	}

	return (0);
}

/**
 * display_prompt - Displays the shell prompt
 *
 * This function prints the prompt symbol (e.g., "$ ") to indicate
 * the shell is ready to accept user input.
 */
void display_prompt(void)
{
	printf("$ ");
	fflush(stdout);
}

/**
 * get_input - Reads input from the user
 *
 * This function uses getline to read a line of input from stdin.
 * If the user inputs EOF (Ctrl+D), the function returns NULL.
 *
 * Return: Pointer to the input string, or NULL on EOF
 */
char *get_input(void)
{
	char *input = NULL;
	size_t input_size = 0;
	ssize_t input_read;

	/* Read a line of input */
	input_read = getline(&input, &input_size, stdin);
	if (input_read == EOF)
	{
		free(input);
		return (NULL);
	}

	/* Remove the newline character if present */
	if (input_read > 0 && input[input_read - 1] == '\n')
	input[input_read - 1] = '\0';

	return (input);
}

/**
 * execute_command - Processes and executes a command
 * @input: The command entered by the user
 *
 * This function checks the input command and performs the corresponding
 * action. It handles built-in commands like "exit" and "env" or
 * reports unknown commands.
 */
void execute_command(char *input)
{
	char *args[64];
	char *token;
	int i = 0;
	char *cmd_path;

	/* Check if input is NULL */
	if (!input)
		return;

	/* Skip leading spaces in input */
	while (input[i] == ' ')
		i++;

	/* Check if remaining input is empty */
	if (input[i] == '\0')
		return;

	/* Handle 'exit' command */
	if (strcmp(&input[i], "exit") == 0)
	{
		free(input);
		exit(0);
	}

	/* Handle 'env' command */
	if (strcmp(&input[i], "env") == 0)
	{
		handle_env();
		return;
	}

	/* Split input into command and arguments */
	token = strtok(&input[i], " ");
	i = 0;
	while (token && i < 63)
	{
		args[i] = token;
		token = strtok(NULL, " ");
		i++;
	}
	args[i] = NULL;

	/* Check if command exists before trying to execute */
	if (args[0] == NULL || args[0][0] == '\0')
		return;

	/* Check if command exists before forking */
	cmd_path = get_command_path(args[0]);
	if (!cmd_path)
	{
		fprintf(stderr, "./hsh: 1: %s: not found\n", args[0]);
		free(input);
		exit(127);
	}

	/* Execute command */
	execute_builtin(args[0], args);
}

/**
 * handle_env - Prints the environment variables
 *
 * This function iterates through the global environment variable array
 * and prints each variable to the standard output.
 */
void handle_env(void)
{
	char **env;

	for (env = environ; *env != NULL; env++)
		printf("%s\n", *env);
}
