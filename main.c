#include "shell.h"

/**
 * main - Simple shell main function
 * @argc: Argument count
 * @argv: Argument vector
 * Return: Exit status
 */
int main(int argc, char **argv)
{
	char *line;
	(void)argc;

	while (1)
	{
		if (isatty(STDIN_FILENO))
			display_prompt();

		line = read_line();
		if (!line)
			break;

		execute_command(line, argv[0]);
		free(line);

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

/**
 * read_line - Read a line from stdin
 * Return: The line read, or NULL on EOF/error
 */
char *read_line(void)
{
	char *line = NULL;
	size_t bufsize = 0;
	ssize_t chars_read;

	chars_read = getline(&line, &bufsize, stdin);
	if (chars_read == -1)
	{
		free(line);
		return (NULL);
	}

	if (line[chars_read - 1] == '\n')
		line[chars_read - 1] = '\0';

	return (line);
}

