#include "shell.h"

/**
 * display_prompt - Display shell prompt
 */
void display_prompt(void)
{
	if (isatty(STDIN_FILENO))
		printf("#cisfun$ ");
}

/**
 * read_input - Read input from stdin
 * @buffer: Buffer to store input
 * @size: Size of buffer
 * Return: Number of characters read
 */
ssize_t read_input(char *buffer, size_t size)
{
	ssize_t chars_read = getline(&buffer, &size, stdin);

	if (chars_read == -1)
	{
		if (isatty(STDIN_FILENO))
			printf("\n");
		return (-1);
	}

	return (chars_read);
}

/**
 * process_line - Process input line
 * @line: Input line to process
 * @program_name: Name of the shell program
 */
void process_line(char *line, char *program_name)
{
	char *line_copy = NULL;
	char *command = NULL;

	if (!line)
		return;

	line_copy = strdup(line);
	if (!line_copy)
		return;

	command = strtok(line_copy, "\n");
	while (command != NULL)
	{
		if (strlen(command) > 0)
		{
			execute_command(command, program_name);
		}
		command = strtok(NULL, "\n");
	}

	free(line_copy);
}

/**
 * main - Entry point
 * @argc: Argument count
 * @argv: Argument vector
 * Return: Exit status
 */
int main(int argc, char **argv)
{
	char *line = NULL;
	size_t len = 0;
	char *program_name = argv[0];

	(void)argc;

	while (1)
	{
		display_prompt();

		if (read_input(line, len) == -1)
			break;

		process_line(line, program_name);
	}

	free(line);
	return (0);
}

