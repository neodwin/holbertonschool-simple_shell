#include "shell.h"

/**
 * handle_env - Prints the environment variables
 *
 * This function iterates through the global environment variable array
 * and prints each variable to the standard output.
 */
void handle_env(void)
{
	char **env;

	for (env = environ; *env != NULL; env++)
		printf("%s\n", *env);
}

/**
 * process_builtin - Process built-in commands
 * @input: Current position in input string
 *
 * Return: 1 if builtin was handled, 0 otherwise
 */
int process_builtin(char *input)
{
	if (strcmp(input, "exit") == 0)
	{
		free(input);
		exit(0);
	}
	if (strcmp(input, "env") == 0)
	{
		handle_env();
		return (1);
	}
	return (0);
}

/**
 * execute_command_child - Execute command in child process
 * @cmd_path: Full path to command
 * @args: Command arguments
 *
 * Return: void, calls _exit
 */
void execute_command_child(char *cmd_path, char **args)
{
	execve(cmd_path, args, environ);
	fprintf(stderr, "%s: 1: %s: not found\n", args[0], args[0]);
	free(cmd_path);
	_exit(127);
}

