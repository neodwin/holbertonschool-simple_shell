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
		fprintf(stderr, "%s: 1: %s: not found\n", program_name, args[0]);
		free(cmd_path);
		exit(127);
	}
}

/**
 * execute_command - Execute a command with arguments
 * @args: Array of arguments
 * @program_name: Name of the shell program
 * Return: Exit status of the command
 */
int execute_command(char **args, char *program_name)
{
	pid_t pid;
	int status;
	char *cmd_path;

	if (!args || !args[0])
		return (0);

	if (handle_builtin(args))
		return (0);

	cmd_path = get_path(args[0]);
	if (!cmd_path)
	{
		fprintf(stderr, "%s: 1: %s: not found\n", program_name, args[0]);
		return (127);
	}

	pid = fork();
	if (pid == -1)
	{
		perror("fork");
		free(cmd_path);
		return (1);
	}

	if (pid == 0)
		execute_in_child(cmd_path, args, program_name);

	waitpid(pid, &status, 0);
	free(cmd_path);
	if (WIFEXITED(status))
		return (WEXITSTATUS(status));
	return (0);
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
			fprintf(stderr, "./hsh: 1: exit: Illegal number: %s\n", args[1]);
			status = 2;
		}
	}
	free_args(args);
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

