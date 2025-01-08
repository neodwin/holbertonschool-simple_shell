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
	size_t pos = 0;
	char c;

	if (!buffer || size == 0)
		return (-1);

	while (pos < size - 1)
	{
		bytes_read = read(STDIN_FILENO, &c, 1);
		if (bytes_read <= 0)
			return (pos == 0 ? bytes_read : (ssize_t)pos);

		buffer[pos++] = c;
		if (c == '\n')
		{
			buffer[pos] = '\0';
			return (pos);
		}
	}
	buffer[pos] = '\0';
	return (pos);
}

/**
 * process_command - Process and execute a single command
 * @command: Command string to process
 * @program_name: Name of the shell program
 * Return: Status of command execution
 */
int process_command(char *command, char *program_name)
{
	char *cmd = strdup(command);
	int status;

	if (!cmd)
		return (-1);

	if (cmd[strlen(cmd) - 1] == '\n')
		cmd[strlen(cmd) - 1] = '\0';

	status = execute_command(cmd, program_name);
	free(cmd);
	return (status);
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
	char *line;
	ssize_t bytes_read;
	(void)argc;

	while (1)
	{
		if (isatty(STDIN_FILENO))
			display_prompt();

		bytes_read = read_input(buffer, sizeof(buffer));
		if (bytes_read <= 0)
			break;

		line = strtok(buffer, "\n");
		while (line)
		{
			process_command(line, argv[0]);
			line = strtok(NULL, "\n");
		}
	}
	return (0);
}

/**
 * display_prompt - Display shell prompt
 */
void display_prompt(void)
{
	printf("$ ");
	fflush(stdout);
}

