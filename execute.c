#include "shell.h"

/**
 * handle_command_error - Handle command execution error
 * @program_name: Name of the program
 * @command: Command that failed
 */
void handle_command_error(const char *program_name, const char *command)
{
	fprintf(stderr, "%s: 1: %s: not found\n", program_name, command);
}

/**
 * execute_builtin - Execute builtin command
 * @command: Command to execute
 * @args: Command arguments
 *
 * Return: 0 on success, 1 on failure
 */
int execute_builtin(char *command, char **args)
{
	char *cmd_path;
	int status;
	char *cmd_copy;
	char *space;

	cmd_copy = strdup(command);
	if (!cmd_copy)
		return (1);

	/* Isolate command from arguments */
	space = strchr(cmd_copy, ' ');
	if (space)
		*space = '\0';

	if (is_ls_command(cmd_copy))
	{
		free(cmd_copy);
		return (execute_ls(command, args));
	}

	cmd_path = get_command_path_ext(cmd_copy);
	free(cmd_copy);

	if (!cmd_path)
		return (1);

	status = execute_command_ext(cmd_path, args);
	return (status);
}

/**
 * prepare_command - Prepare command for execution
 * @line: Input line
 * @args: Array to store arguments
 * @program_name: Name of the program
 *
 * Return: Number of arguments or -1 on failure
 */
int prepare_command(char *line, char **args, char *program_name)
{
	char *line_copy = strdup(line);
	int arg_count;

	if (!line_copy)
		return (-1);

	arg_count = parse_args(line_copy, args, line);
	if (arg_count == 0)
	{
		free(line_copy);
		return (-1);
	}

	args[0] = program_name;
	free(line_copy);
	return (arg_count);
}

/**
 * process_single_command - Process a single command
 * @line: Command line to process
 * @args: Array to store arguments
 * @program_name: Name of the program
 *
 * Return: Command execution status
 */
int process_single_command(char *line, char **args, char *program_name)
{
	int arg_count;
	int status;

	arg_count = prepare_command(line, args, program_name);
	if (arg_count == -1)
		return (0);

	if (process_builtin(line))
		return (0);

	status = execute_builtin(line, args);
	if (status != 0)
		handle_command_error(program_name, line);

	return (status);
}

/**
 * execute_command - Execute command line
 * @input: Command line to execute
 * @program_name: Name of the program
 *
 * Return: Command execution status
 */
int execute_command(char *input, char *program_name)
{
	char *line;
	char *next_line;
	int status = 0;
	char *args[64];

	line = input;
	while (line)
	{
		next_line = find_newline(line);
		if (next_line)
			*next_line = '\0';

		status = process_single_command(line, args, program_name);

		if (next_line)
			line = next_line + 1;
		else
			break;
	}

	return (status);
}

