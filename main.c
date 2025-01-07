#include "shell.h"

/**
 * main - Entry point for the shell
 * @argc: Argument count
 * @argv: Argument vector
 *
 * Return: Last command's exit status
 */
int main(int argc, char **argv)
{
	char *input;
	int last_status = 0;
	(void)argc;

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
		last_status = execute_command(input, argv[0]);
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
	char *temp = NULL;
	size_t input_size = 0;
	size_t total_size = 0;
	ssize_t input_read;

	while ((input_read = getline(&temp, &input_size, stdin)) != EOF)
	{
		if (input == NULL)
		{
			input = strdup(temp);
			if (!input)
			{
				free(temp);
				return (NULL);
			}
			total_size = strlen(input) + 1;
		}
		else
		{
			char *new_input = realloc(input, total_size + input_read);
			if (!new_input)
			{
				free(input);
				free(temp);
				return (NULL);
			}
			input = new_input;
			strcat(input, temp);
			total_size += input_read;
		}
	}

	free(temp);
	if (!input)
		return (NULL);

	return (input);
}
