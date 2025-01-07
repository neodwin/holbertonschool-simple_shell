#include "shell.h"

/**
 * resolve_dots - Resolve dots in path
 * @path: Path to resolve
 *
 * Return: Resolved path (must be freed) or NULL
 */
char *resolve_dots(const char *path)
{
	char *resolved = strdup(path);
	char *token;
	char *result;
	int len = 0;

	if (!resolved)
		return (NULL);

	result = malloc(strlen(path) + 1);
	if (!result)
	{
		free(resolved);
		return (NULL);
	}
	result[0] = '\0';

	token = strtok(resolved, "/");
	while (token)
	{
		if (strcmp(token, ".") == 0)
			;
		else if (strcmp(token, "..") == 0)
			len = strlen(result) - 1;
		else
		{
			if (len > 0)
				strcat(result, "/");
			strcat(result, token);
			len = strlen(result);
		}
		token = strtok(NULL, "/");
	}

	free(resolved);
	return (result);
}

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
	struct stat st;

	/* Handle ls command specially */
	if (is_ls_command(command))
		return (execute_ls(command, args));

	/* Try direct path first */
	if (command[0] == '/')
		cmd_path = strdup(command);
	else if (command[0] == '.')
		cmd_path = resolve_dots(command);
	else
		cmd_path = get_command_path(command);

	if (!cmd_path)
	{
		fprintf(stderr, "%s: 1: %s: not found\n", args[0], command);
		return (127);
	}

	/* Check if file exists and is executable */
	if (stat(cmd_path, &st) != 0 || !(st.st_mode & S_IXUSR))
	{
		fprintf(stderr, "%s: 1: %s: not found\n", args[0], command);
		free(cmd_path);
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

