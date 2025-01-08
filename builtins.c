#include "shell.h"

/**
 * handle_exit - Handle exit builtin command
 */
void handle_exit(void)
{
	exit(0);
}

/**
 * handle_builtin - Handle builtin commands
 * @command: Command to check
 * Return: 1 if builtin was handled, 0 otherwise
 */
int handle_builtin(char *command)
{
	if (strcmp(command, "exit") == 0)
	{
		handle_exit();
		return (1);
	}
	return (0);
}
