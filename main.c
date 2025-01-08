#include "shell.h"

/**
 * read_input - Read input from stdin
 * @buffer: Buffer to store input
 * @size: Size of buffer
 * Return: Number of bytes read or -1 on error
 */
ssize_t read_input(char *buffer, size_t size)
{
	ssize_t total_read = 0;
	ssize_t bytes_read;

	bytes_read = read(STDIN_FILENO, buffer, size - 1);
	if (bytes_read > 0)
	{
		total_read = bytes_read;
		buffer[total_read] = '\0';
	}
	return (bytes_read);
}

/**
 * trim_whitespace - Remove leading and trailing whitespace
 * @str: String to trim
 * Return: Pointer to trimmed string
 */
char *trim_whitespace(char *str)
{
	char *end;

	while (*str && (*str == ' ' || *str == '\t' || *str == '\n'))
		str++;

	if (*str == '\0')
		return (str);

	end = str + strlen(str) - 1;
	while (end > str && (*end == ' ' || *end == '\t' || *end == '\n'))
		end--;

	*(end + 1) = '\0';
	return (str);
}

/**
 * process_commands - Process multiple commands from input
 * @input: Input string containing commands
 * @program_name: Name of the shell program
 */
void process_commands(char *input, char *program_name)
{
	char *command;
	char *saveptr;

	command = strtok_r(input, "\n", &saveptr);
	while (command != NULL)
	{
		command = trim_whitespace(command);
		if (*command != '\0')
			execute_command(command, program_name);
		command = strtok_r(NULL, "\n", &saveptr);
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

		process_commands(buffer, argv[0]);

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

