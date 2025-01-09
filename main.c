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
	/* Initialize counters for bytes read */
	ssize_t bytes_read = 0;
	ssize_t total_read = 0;
	/* Character buffer for reading one byte at a time */
	char c;

	/* Read until buffer is almost full */
	while (total_read < (ssize_t)(size - 1))
	{
		/* Read a single character from standard input */
		bytes_read = read(STDIN_FILENO, &c, 1);
		/* Return if EOF or error */
		if (bytes_read <= 0)
			return (bytes_read);

		/* Handle newline character */
		if (c == '\n')
		{
			/* Null-terminate the string */
			buffer[total_read] = '\0';
			/* Return if we have read something */
			if (total_read > 0)
				return (total_read);
			/* Skip empty lines */
			continue;
		}

		/* Store character and increment counter */
		buffer[total_read++] = c;
	}
	/* Ensure null-termination and return */
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
	/* Buffer to store user input */
	char buffer[1024];
	/* Variable to track number of bytes read */
	ssize_t bytes_read;

	/* Main interactive loop */
	while (1)
	{
		/* Display shell prompt */
		display_prompt();
		/* Read user input */
		bytes_read = read_input(buffer, sizeof(buffer));
		/* Exit loop if EOF or error */
		if (bytes_read <= 0)
			break;
		/* Execute the command */
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
	/* Buffer to store input commands */
	char buffer[1024];
	/* Variable to store number of bytes read */
	ssize_t bytes_read;
	/* Counter for buffer traversal */
	int i;

	/* Main processing loop */
	while (1)
	{
		/* Read input into buffer */
		bytes_read = read_input(buffer, sizeof(buffer));
		/* Exit if EOF or error */
		if (bytes_read <= 0)
			break;

		/* Check for non-whitespace characters */
		for (i = 0; buffer[i] != '\0'; i++)
		{
			/* If valid character found, execute command */
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
	/* Suppress unused parameter warning */
	(void)argc;

	/* Check if input is from terminal */
	if (isatty(STDIN_FILENO))
		/* Run in interactive mode */
		interactive_mode(argv[0]);
	else
		/* Run in non-interactive mode */
		non_interactive_mode(argv[0]);

	/* Return success */
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
	/* Print shell prompt */
	printf("$ ");
	/* Ensure prompt is displayed immediately */
	fflush(stdout);
}

