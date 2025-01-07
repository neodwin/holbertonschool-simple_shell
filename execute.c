#include "shell.h"

/**
 * execute_in_child - Execute command in child process
 * @cmd_path: Path to command
 * @args: Command arguments
 * @program_name: Name of shell program
 */
void execute_in_child(char *cmd_path, char **args, char *program_name)
{
	if (execve(cmd_path, args, environ) == -1)
	{
		fprintf(stderr, "%s: 1: %s: not found\n",
			program_name, args[0]);
		free(cmd_path);
		exit(127);
	}
}

/**
 * parse_command - Parse command into arguments
 * @command: Command string to parse
 * Return: Array of arguments
 */
char **parse_command(char *command)
{
	char **args = malloc(sizeof(char *) * 32);
	char *token;
	int i = 0;

	if (!args)
		return (NULL);

	token = strtok(command, " \t\n");
	while (token && i < 31)
	{
		args[i] = strdup(token);
		if (!args[i])
		{
			while (--i >= 0)
				free(args[i]);
			free(args);
			return (NULL);
		}
		i++;
		token = strtok(NULL, " \t\n");
	}
	args[i] = NULL;
	return (args);
}

/**
 * cleanup - Free allocated memory
 * @cmd_path: Command path to free
 * @args: Arguments array to free
 */
void cleanup(char *cmd_path, char **args)
{
	int i;

	if (cmd_path)
		free(cmd_path);
	if (args)
	{
		for (i = 0; args[i]; i++)
			free(args[i]);
		free(args);
	}
}

/**
 * prepare_command - Prepare command for execution
 * @command: Command string
 * Return: Parsed arguments array or NULL on error
 */
char **prepare_command(char *command)
{
	char **args;

	while (*command == ' ' || *command == '\t')
		command++;

	if (*command == '\0')
		return (NULL);

	args = parse_command(command);
	if (!args)
		return (NULL);

	if (handle_builtin(args))
	{
		cleanup(NULL, args);
		return (NULL);
	}

	return (args);
}

/**
 * execute_command - Execute a command
 * @command: Command to execute
 * @program_name: Name of the shell program
 */
void execute_command(char *command, char *program_name)
{
	pid_t pid;
	int status;
	char **args;
	char *cmd_path = NULL;

	args = prepare_command(command);
	if (!args)
		return;

	cmd_path = get_path(args[0]);
	if (!cmd_path)
	{
		fprintf(stderr, "%s: 1: %s: not found\n",
			program_name, args[0]);
		cleanup(cmd_path, args);
		exit(127);
	}

	pid = fork();
	if (pid == -1)
	{
		perror("fork");
		cleanup(cmd_path, args);
		return;
	}

	if (pid == 0)
		execute_in_child(cmd_path, args, program_name);

	waitpid(pid, &status, 0);
	cleanup(cmd_path, args);
}

