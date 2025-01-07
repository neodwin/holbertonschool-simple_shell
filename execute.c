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
	pid_t pid;
	char *cmd_path;
	int status = 0;

	/* Handle ls command specially */
	if (is_ls_command(command))
		return (execute_ls(command, args));

	/* Get full path of command */
	cmd_path = get_command_path(command);
	if (!cmd_path)
	{
		fprintf(stderr, "%s: 1: %s: not found\n", args[0], args[0]);
		return (127);
	}

	/* Create new process only if command exists */
	pid = fork();
	if (pid == -1)
	{
			free(cmd_path);
			perror("fork");
			return (1);
	}

	if (pid == 0)
		execute_command_child(cmd_path, args);
	else
	{
		waitpid(pid, &status, 0);
		free(cmd_path);
		if (WIFEXITED(status))
			return (WEXITSTATUS(status));
	}
	return (status);
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
	char *token;
	int i = 0;
	char *line_copy = strdup(line);
	char *start;

	if (!line_copy)
		return (0);

	/* Skip leading spaces */
	start = line_copy;
	while (*start == ' ' || *start == '\t')
		start++;

	/* Copy cleaned start back to line_copy */
	if (start != line_copy)
		memmove(line_copy, start, strlen(start) + 1);

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

	free(line_copy);
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

		if (*line && !process_builtin(line))
		{
			if (parse_command(line, args) > 0 && args[0][0] != '\0')
			{
				args[0] = program_name;
				status = execute_builtin(args[0], args);
			}
		}

		if (next_line)
			line = next_line + 1;
		else
			break;
	}

	free(input_copy);
	return (status);
}

