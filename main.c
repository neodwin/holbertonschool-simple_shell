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
	char **args;
	int status = 0;
	(void)argc;

	while (1)
	{
		if (isatty(STDIN_FILENO))
			display_prompt();

		line = read_line();
		if (!line)
			break;

		args = split_line(line);
		if (args)
		{
			status = execute_command(args, argv[0]);
			free_args(args);
		}
		free(line);

		if (!isatty(STDIN_FILENO))
			break;
	}
	return (status);
}

/**
 * display_prompt - Display shell prompt
 */
void display_prompt(void)
{
	printf("$ ");
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
/**
 * split_line - Split a line into arguments
 * @line: The line to split
 * Return: Array of arguments
 */
char **split_line(char *line)
{
	char **args = malloc(sizeof(char *) * 64);
	char *token;
	int i = 0;

	if (!args)
		return (NULL);

	token = strtok(line, " \t\n");
	while (token && i < 63)
	{
		args[i] = strdup(token);
		if (!args[i])
		{
			free_args(args);
			return (NULL);
		}
		i++;
		token = strtok(NULL, " \t\n");
	}
	args[i] = NULL;

	return (args);
}

/**
 * free_args - Free argument array
 * @args: Array of arguments to free
 */
void free_args(char **args)
{
	int i;

	if (!args)
		return;

	for (i = 0; args[i]; i++)
		free(args[i]);
	free(args);
}

