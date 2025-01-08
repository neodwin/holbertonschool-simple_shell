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
		if (bytes_read <= 0)
			return (bytes_read);

		if (c == '\n')
		{
			buffer[total_read] = '\0';
			return (total_read);
		}

		buffer[total_read++] = c;
	}
	buffer[total_read] = '\0';
	return (total_read);
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

		if (bytes_read > 0)
			execute_command(buffer, argv[0]);

		if (!isatty(STDIN_FILENO))
			continue;
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

