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
	/* Initialize exit status to 0 */
	int status = 0;
	/* Counter for digit validation */
	int i;

	/* Check if status argument is provided */
	if (args[1])
	{
		/* Reset status for conversion */
		status = 0;
		/* Process each character of the status argument */
		for (i = 0; args[1][i]; i++)
		{
			/* Validate that character is a digit */
			if (args[1][i] < '0' || args[1][i] > '9')
			{
				/* Print error for non-numeric status */
				fprintf(stderr, "./hsh: 1: exit: Illegal number: %s\n",
					args[1]);
				/* Set error status */
				status = 2;
				break;
			}
			/* Convert string to integer digit by digit */
			status = status * 10 + (args[1][i] - '0');
		}
	}

	/* Clean up memory before exit */
	cleanup(NULL, args);
	/* Exit with computed status */
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
	/* Pointer for environment variable traversal */
	char **env;

	/* Check for NULL arguments */
	if (!args || !args[0])
		return (0);

	/* Check for exit command */
	if (strcmp(args[0], "exit") == 0)
	{
		/* Handle exit command */
		handle_exit(args);
		return (1);
	}

	/* Check for env command */
	if (strcmp(args[0], "env") == 0)
	{
		/* Print each environment variable */
		for (env = environ; *env; env++)
			printf("%s\n", *env);
		return (1);
	}

	/* Return 0 if not a builtin command */
	return (0);
}
