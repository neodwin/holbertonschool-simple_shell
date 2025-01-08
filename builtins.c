#include "shell.h"

/**
 * handle_exit - Handle the exit built-in command
 * @args: Array of command arguments (args[1] may contain exit status)
 *
 * Description: This function handles the 'exit' built-in command. It can:
 * 1. Exit with status 0 if no argument is provided
 * 2. Exit with the specified status if a valid number is provided
 * 3. Print an error and exit with status 2 if an invalid number is provided
 * The function validates the exit status to ensure it's a valid number.
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
 * handle_builtin - Handle built-in shell commands
 * @args: Array of command arguments (args[0] is the command name)
 *
 * Description: This function checks if a command is a built-in and executes
 * it if it is. Currently supported built-ins are:
 * - exit: Exits the shell with an optional status
 * - env: Prints the current environment variables
 * The function returns 1 if a built-in was handled, 0 otherwise.
 *
 * Return: 1 if built-in was handled, 0 if not a built-in
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
