#include "shell.h"

/**
 * main - Entry point for the shell
 *
 * This function initializes the shell, handles user input, and
 * executes commands in a loop until the user exits.
 *
 * Return: Last command's exit status
 */
int main(void)
{
	char *input;
	int last_status = 0;

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
		last_status = execute_command(input);
		free(input);
		if (!isatty(STDIN_FILENO))
			exit(last_status);
	}

	return (last_status);
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

	return (input);
}
