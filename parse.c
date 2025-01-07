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

	/* Remove trailing spaces */
	start = line_copy + strlen(line_copy) - 1;
	while (start > line_copy && (*start == ' ' || *start == '\t' ||
		*start == '\n'))
		*start-- = '\0';

	/* Split command and arguments */
	token = strtok(line_copy, " \t");
	if (token)
	{
		offset = token - line_copy;
		args[i++] = line + offset;

		/* Parse remaining arguments */
		while ((token = strtok(NULL, " \t")) && i < 63)
		{
			if (*token)
			{
				offset = token - line_copy;
				args[i++] = line + offset;
			}
		}
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
	char *token;
	int i = 0;

	if (!line_copy)
		return (0);

	/* Skip leading spaces */
	while (*line_copy == ' ' || *line_copy == '\t')
		line_copy++;

	/* Remove trailing newline and spaces */
	token = line_copy + strlen(line_copy) - 1;
	while (token > line_copy && (*token == ' ' || *token == '\t' ||
		*token == '\n'))
		*token-- = '\0';

	/* Parse command and arguments */
	token = strtok(line_copy, " \t");
	while (token && i < 63)
	{
		args[i++] = token;
		token = strtok(NULL, " \t");
	}
	args[i] = NULL;

	return (i);
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
