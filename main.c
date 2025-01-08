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
			if (total_read > 0)
				return (total_read);
			continue;
		}

		buffer[total_read++] = c;
	}
	buffer[total_read] = '\0';
	return (total_read);
}

/**
 * interactive_mode - Handle interactive mode
 * @program_name: Name of the program
 */
void interactive_mode(char *program_name)
{
	char buffer[1024];
	ssize_t bytes_read;

	while (1)
	{
		display_prompt();
		bytes_read = read_input(buffer, sizeof(buffer));
		if (bytes_read <= 0)
			break;
		execute_command(buffer, program_name);
	}
}

/**
 * non_interactive_mode - Handle non-interactive mode
 * @program_name: Name of the program
 */
void non_interactive_mode(char *program_name)
{
	char buffer[1024];
	ssize_t bytes_read;
	int i;

	while (1)
	{
		bytes_read = read_input(buffer, sizeof(buffer));
		if (bytes_read <= 0)
			break;

		for (i = 0; buffer[i] != '\0'; i++)
		{
			if (buffer[i] != ' ' && buffer[i] != '\t')
			{
				execute_command(buffer, program_name);
				break;
			}
		}
	}
}

/**
 * main - Simple shell main function
 * @argc: Argument count
 * @argv: Argument vector
 * Return: Exit status
 */
int main(int argc, char **argv)
{
	(void)argc;

	if (isatty(STDIN_FILENO))
		interactive_mode(argv[0]);
	else
		non_interactive_mode(argv[0]);

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

