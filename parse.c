#include "shell.h"

/**
 * parse_args - Helper function to parse arguments
 * @line_copy: Copy of input line
 * @args: Array to store arguments
 * @line: Original input line
 *
 * Return: Number of arguments parsed
 */
int parse_args(char *line_copy, char **args, char *line)
{
	char *token;
	int i = 0;

	token = strtok(line_copy, " \t");
	while (token && i < 63)
	{
		args[i] = token;
		token = strtok(NULL, " \t");
		i++;
	}
	args[i] = NULL;

	/* Copy arguments back to original line */
	for (i = 0; args[i]; i++)
		args[i] = line + (args[i] - line_copy);

	return (i);
}

/**
 * parse_command - Parse command line into arguments
 * @line: Command line to parse
 * @args: Array to store arguments
 *
 * Return: Number of arguments parsed
 */
int parse_command(char *line, char **args)
{
	char *line_copy = strdup(line);
	char *start;
	int result;

	if (!line_copy)
		return (0);

	/* Skip leading spaces */
	start = line_copy;
	while (*start == ' ' || *start == '\t')
		start++;

	/* Copy cleaned start back to line_copy */
	if (start != line_copy)
		memmove(line_copy, start, strlen(start) + 1);

	result = parse_args(line_copy, args, line);
	free(line_copy);
	return (result);
}

/**
 * find_newline - Find next newline character
 * @str: String to search
 *
 * Return: Pointer to newline or NULL if not found
 */
char *find_newline(char *str)
{
	while (*str)
	{
		if (*str == '\n')
			return (str);
		str++;
	}
	return (NULL);
}
