#include "shell.h"

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
