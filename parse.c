#include "shell.h"

/**
 * is_empty_line - Check if a line is empty or contains only whitespace
 * @line: Line to check
 *
 * Return: 1 if line is empty, 0 otherwise
 */
int is_empty_line(const char *line)
{
	while (*line)
	{
		if (*line != ' ' && *line != '\t' && *line != '\n')
			return (0);
		line++;
	}
	return (1);
}

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

	if (is_empty_line(line_copy))
		return (0);

	/* Skip leading spaces */
	while (*start == ' ' || *start == '\t')
		start++;

	/* Move content to beginning of buffer */
	if (start != line_copy)
		memmove(line_copy, start, strlen(start) + 1);

	/* Remove trailing spaces and newline */
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
			offset = token - line_copy;
			args[i++] = line + offset;
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
	int i = 0;

	while (str[i])
	{
		if (str[i] == '\n')
			return (&str[i]);
		i++;
	}
	return (NULL);
}
