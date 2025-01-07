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
		fprintf(stderr, "%s: No such file or directory\n",
			program_name);
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
		args[i] = token;
		i++;
		token = strtok(NULL, " \t\n");
	}
	args[i] = NULL;
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
	char *cmd_path;

	/* Remove leading/trailing whitespace */
	while (*command == ' ' || *command == '\t')
		command++;

	if (*command == '\0')
		return;

	args = parse_command(command);
	if (!args)
		return;

	cmd_path = get_path(args[0]);
	if (!cmd_path)
	{
		fprintf(stderr, "%s: No such file or directory\n", program_name);
		free(args);
		return;
	}

	pid = fork();
	if (pid == -1)
	{
		perror("fork");
		free(cmd_path);
		free(args);
		return;
	}

	if (pid == 0)
		execute_in_child(cmd_path, args, program_name);
	else
	{
		waitpid(pid, &status, 0);
		free(cmd_path);
		free(args);
	}
}

/**
 * handle_exit - Handle exit builtin command
 * @args: Command arguments
 */
void handle_exit(char **args)
{
	int status = 0;

	if (args[1])
	{
		status = atoi(args[1]);
		if (status < 0)
		{
			fprintf(stderr, "./hsh: 1: exit: Illegal number: %s\n",
				args[1]);
			status = 2;
		}
	}
	exit(status);
}

/**
 * handle_builtin - Handle built-in commands
 * @args: Array of arguments
 * Return: 1 if builtin was handled, 0 otherwise
 */
int handle_builtin(char **args)
{
	int i;

	if (!args || !args[0])
		return (0);

	if (strcmp(args[0], "exit") == 0)
	{
		handle_exit(args);
	}

	if (strcmp(args[0], "env") == 0)
	{
		for (i = 0; environ[i]; i++)
			printf("%s\n", environ[i]);
		return (1);
	}

	return (0);
}

