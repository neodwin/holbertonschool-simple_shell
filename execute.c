#include "shell.h"

/**
 * execute_in_child - Execute command in child process
 * @command: Command to execute
 * @args: Command arguments
 * @program_name: Name of the shell program
 * Return: Exit status of command
 */
int execute_in_child(char *command, char **args, char *program_name)
{
	pid_t pid;
	int status;

	pid = fork();
	if (pid == -1)
	{
		perror(program_name);
		return (1);
	}
	if (pid == 0)
	{
		if (execve(command, args, environ) == -1)
		{
			perror(program_name);
			exit(127);
		}
	}
	else
	{
		waitpid(pid, &status, 0);
		if (WIFEXITED(status))
			return (WEXITSTATUS(status));
	}
	return (0);
}

/**
 * parse_command - Parse command into arguments
 * @command: Command string to parse
 * Return: Array of arguments
 */
char **parse_command(char *command)
{
	char **args = malloc(sizeof(char *) * MAX_ARGS);
	char *cmd_copy, *token;
	int i = 0;

	if (!args)
		return (NULL);

	cmd_copy = strdup(command);
	if (!cmd_copy)
	{
		free(args);
		return (NULL);
	}

	token = strtok(cmd_copy, " \t\n");
	while (token && i < MAX_ARGS - 1)
	{
		args[i] = strdup(token);
		if (!args[i])
		{
			cleanup(args);
			free(cmd_copy);
			return (NULL);
		}
		i++;
		token = strtok(NULL, " \t\n");
	}
	args[i] = NULL;
	free(cmd_copy);
	return (args);
}

/**
 * cleanup - Free memory used by arguments array
 * @args: Arguments array to free
 */
void cleanup(char **args)
{
	int i;

	if (!args)
		return;

	for (i = 0; args[i]; i++)
		free(args[i]);
	free(args);
}

/**
 * prepare_command - Prepare command by trimming spaces
 * @command: Command string to prepare
 * Return: Prepared command string
 */
char *prepare_command(char *command)
{
	char *start = command;
	char *end;

	if (!command)
		return (NULL);

	/* Skip leading spaces */
	while (*start && (*start == ' ' || *start == '\t'))
		start++;

	if (!*start)
		return (NULL);

	/* Find end of string and trim trailing spaces */
	end = start + strlen(start) - 1;
	while (end > start && (*end == ' ' || *end == '\t' || *end == '\n'))
		end--;
	*(end + 1) = '\0';

	if (start != command)
		memmove(command, start, strlen(start) + 1);

	return (command);
}

/**
 * execute_command - Execute a command
 * @command: Command to execute
 * @program_name: Name of the shell program
 * Return: Exit status of command
 */
int execute_command(char *command, char *program_name)
{
	char **args;
	char *cmd_path;
	int status = 0;

	command = prepare_command(command);
	if (!command || !*command)
		return (0);

	if (handle_builtin(command))
		return (0);

	args = parse_command(command);
	if (!args || !args[0])
	{
		cleanup(args);
		return (0);
	}

	cmd_path = get_path(args[0], environ);
	if (!cmd_path)
	{
		fprintf(stderr, "%s: 1: %s: not found\n", program_name, args[0]);
		cleanup(args);
		return (127);
	}

	status = execute_in_child(cmd_path, args, program_name);
	free(cmd_path);
	cleanup(args);
	return (status);
}

