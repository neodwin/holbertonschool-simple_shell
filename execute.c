#include "shell.h"

/**
 * execute_builtin - Execute a command with its arguments
 * @command: The command to execute
 * @args: Array of command and its arguments
 *
 * Return: Exit status of the command (127 if not found, 0 on success)
 */
int execute_builtin(char *command, char **args)
{
	char *cmd_path;

	/* Handle ls command specially */
	if (is_ls_command(command))
		return (execute_ls(command, args));

	/* Get command path with extended checks */
	cmd_path = get_command_path_ext(command);
	if (!cmd_path)
	{
		fprintf(stderr, "%s: 1: %s: not found\n", args[0], command);
		return (127);
	}

	return (execute_command_ext(cmd_path, args));
}

/**
 * process_single_command - Process and execute a single command
 * @line: Command line to process
 * @args: Array to store arguments
 * @program_name: Name of the program
 *
 * Return: Status of command execution
 */
int process_single_command(char *line, char **args, char *program_name)
{
	int status = 0;
	char *original_command;

	if (*line && !process_builtin(line))
	{
		if (parse_command(line, args) > 0 && args[0][0] != '\0')
		{
			original_command = strdup(args[0]);
			if (!original_command)
				return (1);

			args[0] = program_name;
			status = execute_builtin(original_command, args);

			free(original_command);
		}
	}
	return (status);
}

/**
 * execute_command - Processes and executes a command
 * @input: The command entered by the user
 * @program_name: Name of the program (argv[0])
 *
 * Return: Exit status of the command
 */
int execute_command(char *input, char *program_name)
{
	char *args[64];
	int status = 0;
	char *line, *next_line;
	char *input_copy;

	if (!input)
		return (0);

	input_copy = strdup(input);
	if (!input_copy)
		return (0);

	line = input_copy;
	while (line && *line)
	{
		/* Skip empty lines */
		while (*line == '\n' || *line == ' ' || *line == '\t')
			line++;

		if (!*line)
			break;

		next_line = find_newline(line);
		if (next_line)
			*next_line = '\0';

		status = process_single_command(line, args, program_name);

		if (next_line)
			line = next_line + 1;
		else
			break;
	}

	free(input_copy);
	return (status);
}

