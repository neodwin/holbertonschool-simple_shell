#include "shell.h"

/**
 * read_input - Read input from stdin
 * @buffer: Buffer to store input
 * @size: Size of buffer
 * Return: Number of bytes read or -1 on error
 */
ssize_t read_input(char *buffer, size_t size)
{
	ssize_t bytes_read = 0;
	ssize_t total_read = 0;
	char c;

	while (total_read < (ssize_t)(size - 1))
	{
		bytes_read = read(STDIN_FILENO, &c, 1);
		if (bytes_read <= 0 || c == '\n')
			break;

		buffer[total_read++] = c;
	}

	if (total_read > 0 || (bytes_read > 0 && c == '\n'))
	{
		if (c == '\n')
			buffer[total_read++] = c;
		buffer[total_read] = '\0';
	}

	return (total_read);
}

/**
 * process_line - Process a single line of input
 * @line: Line to process
 * @program_name: Name of the program
 */
void process_line(char *line, char *program_name)
{
	char *command;
	char *line_copy = strdup(line);
	char *saveptr = NULL;

	if (!line_copy)
		return;

	command = strtok_r(line_copy, "\n", &saveptr);
	while (command)
	{
		char *tmp = command;

		while (*tmp == ' ' || *tmp == '\t')
			tmp++;
		if (*tmp != '\0')
			execute_command(command, program_name);
		command = strtok_r(NULL, "\n", &saveptr);
	}

	free(line_copy);
}

/**
 * main - Simple shell main function
 * @argc: Argument count
 * @argv: Argument vector
 * Return: Exit status
 */
int main(int argc, char **argv)
{
	char buffer[1024];
	ssize_t bytes_read;
	(void)argc;

	while (1)
	{
		if (isatty(STDIN_FILENO))
			display_prompt();

		bytes_read = read_input(buffer, sizeof(buffer));
		if (bytes_read <= 0)
			break;

		process_line(buffer, argv[0]);

		if (!isatty(STDIN_FILENO))
			break;
	}
	return (0);
}

/**
 * display_prompt - Display shell prompt
 */
void display_prompt(void)
{
	printf("#cisfun$ ");
	fflush(stdout);
}

