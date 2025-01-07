#include "shell.h"

/**
 * process_command_line - Process command line and execute
 * @line: Command line to process
 * @args: Array to store arguments
 * @program_name: Name of the program
 *
 * Return: Command execution status
 */
int process_command_line(char *line, char **args, char *program_name)
{
	char *current_line;
	char *trimmed_line;
	int status = 0;

	current_line = strdup(line);
	if (!current_line)
		return (1);

	trimmed_line = current_line;
	while (*trimmed_line == " " || *trimmed_line == "\t")
		trimmed_line++;

	if (*trimmed_line)
		status = process_single_command(current_line, args, program_name);

	free(current_line);
	return (status);
}
