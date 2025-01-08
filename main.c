#include "shell.h"

/**
 * read_input - Read a line of input from standard input
 * @buffer: Buffer to store the input
 * @size: Size of the buffer
 *
 * Description: Reads input character by character until newline
 * or buffer limit. Handles both interactive and non-interactive
 * modes. Skips empty lines. Ensures proper null-termination.
 *
 * Return: Number of bytes read, or -1 on error/EOF
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
 * interactive_mode - Handle the shell's interactive mode
 * @program_name: Name of the shell program (argv[0])
 *
 * Description: Implements interactive shell mode with prompt
 * display and command processing. Continues until EOF (Ctrl+D)
 * or exit command. Handles each command in a loop.
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
 * non_interactive_mode - Handle the shell's non-interactive mode
 * @program_name: Name of the shell program (argv[0])
 *
 * Description: Processes commands from pipe or file without
 * displaying prompt. Reads until EOF, skips empty lines,
 * executes each valid command, and handles errors.
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
 * main - Entry point for the simple shell program
 * @argc: Argument count (unused)
 * @argv: Argument vector (used for program name)
 *
 * Description: Main entry point that determines mode (interactive
 * or non-interactive) based on input source. Handles command
 * processing accordingly and ensures proper termination.
 *
 * Return: Always returns 0 on normal termination
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
 * display_prompt - Display the shell prompt
 *
 * Description: Displays shell prompt '$ ' and flushes stdout
 * to ensure immediate display. Only used in interactive mode
 * to provide user feedback and command input indication.
 */
void display_prompt(void)
{
	printf("$ ");
	fflush(stdout);
}

