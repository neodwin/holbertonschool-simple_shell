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
	struct stat st;

	cmd_path = get_command_path(command);
	if (!cmd_path)
	{
		fprintf(stderr, "%s: 1: %s: not found\n", args[0], command);
		return (127);
	}

	if (stat(cmd_path, &st) == -1 || !(st.st_mode & S_IXUSR))
	{
		fprintf(stderr, "%s: 1: %s: Permission denied\n", args[0], command);
		free(cmd_path);
		return (126);
	}

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

	token = strtok(line, " \t");
	while (token && i < 63)
	{
		args[i] = token;
		token = strtok(NULL, " \t");
		i++;
	}
	args[i] = NULL;
	return (i);
}

/**
 * execute_command - Processes and executes a command
 * @input: The command entered by the user
 *
 * Return: Exit status of the command
 */
int execute_command(char *input)
{
	char *args[64];
	int status = 0;
	char *saveptr;
	char *line;
	char *input_copy;

	if (!input)
		return (0);

	input_copy = strdup(input);
	if (!input_copy)
		return (0);

	line = strtok_r(input_copy, "\n", &saveptr);
	while (line)
	{
		while (*line == ' ' || *line == '\t')
			line++;

		if (*line)
		{
			if (!process_builtin(line))
			{
				if (parse_command(line, args) > 0 && args[0][0] != '\0')
					status = execute_builtin(args[0], args);
			}
		}
		line = strtok_r(NULL, "\n", &saveptr);
	}

	free(input_copy);
	return (status);
}

