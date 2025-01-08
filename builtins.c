#include "shell.h"

/**
 * handle_exit - Handle the exit built-in command
 * @args: Array of command arguments (args[0] is "exit")
 *
 * Description: Processes exit command with optional status.
 * If no status provided, exits with status 0. If status is
 * provided, validates it and exits with that status. Handles
 * numeric conversion and validation of status argument.
 */
void handle_exit(char **args)
{
	int status = 0;
	int i;

	if (args[1])
	{
		status = 0;
		for (i = 0; args[1][i]; i++)
		{
			if (args[1][i] < '0' || args[1][i] > '9')
			{
				fprintf(stderr, "./hsh: 1: exit: Illegal number: %s\n",
					args[1]);
				status = 2;
				break;
			}
			status = status * 10 + (args[1][i] - '0');
		}
	}

	cleanup(NULL, args);
	exit(status);
}

/**
 * handle_builtin - Handle built-in shell commands
 * @args: Array of command arguments
 *
 * Description: Checks if command is a built-in (exit or env)
 * and executes appropriate handler. For env, prints all
 * environment variables. For exit, calls exit handler.
 *
 * Return: 1 if command was a built-in, 0 otherwise
 */
int handle_builtin(char **args)
{
	char **env;

	if (!args || !args[0])
		return (0);

	if (strcmp(args[0], "exit") == 0)
	{
		handle_exit(args);
		return (1);
	}

	if (strcmp(args[0], "env") == 0)
	{
		for (env = environ; *env; env++)
			printf("%s\n", *env);
		return (1);
	}

	return (0);
}
