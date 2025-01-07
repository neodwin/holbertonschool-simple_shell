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
	char *start = line_copy;
	size_t offset;

	/* Skip leading spaces */
	while (*start == ' ' || *start == '\t')
		start++;

	/* Move content to beginning of buffer */
	if (start != line_copy)
		memmove(line_copy, start, strlen(start) + 1);

	token = strtok(line_copy, " \t");
	while (token && i < 63)
	{
		/* Skip empty tokens */
		if (*token)
		{
			/* Calculate offset in original string */
			offset = token - line_copy;
			args[i] = line + offset;
			i++;
		}
		token = strtok(NULL, " \t");
	}
	args[i] = NULL;

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
	int result;

	if (!line_copy)
		return (0);

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
