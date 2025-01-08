#include "shell.h"

/**
 * execute_in_child - Execute a command in a child process
 * @cmd_path: Full path to the command executable
 * @args: Array of command arguments (including command name)
 * @program_name: Name of the shell program for error messages
 *
 * Description: Executes the command using execve in a child process.
 * If execution fails, prints error message with program name and
 * command, frees allocated memory, and exits with status 127.
 */
void execute_in_child(char *cmd_path, char **args, char *program_name)
{
	if (execve(cmd_path, args, environ) == -1)
	{
		fprintf(stderr, "%s: 1: %s: not found\n", program_name,
			args[0]);
		free(cmd_path);
		exit(127);
	}
}

/**
 * parse_command - Parse a command string into an array of arguments
 * @command: The command string to parse
 *
 * Description: Splits command string into array of arguments using
 * space, tab, and newline as delimiters. Allocates memory for up
 * to 32 arguments. Each argument is duplicated in new memory.
 *
 * Return: Array of strings (arguments) or NULL if allocation fails
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
 * cleanup - Free memory allocated for command execution
 * @cmd_path: The command path string to free
 * @args: The array of argument strings to free
 *
 * Description: Handles memory cleanup after command execution.
 * Frees both the command path string and array of argument
 * strings. Checks for NULL before freeing to prevent errors.
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
 * prepare_command - Prepare a command string for execution
 * @command: The command string to prepare
 *
 * Description: Prepares command by removing leading whitespace,
 * checking if empty, parsing into arguments, and checking if
 * it's a built-in command. Returns NULL for empty/built-in.
 *
 * Return: Array of parsed arguments, or NULL if empty/built-in
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
 * execute_command - Main function to execute a command
 * @command: The command string to execute
 * @program_name: Name of the shell program for error messages
 *
 * Description: Main command execution function that handles:
 * 1. Command preparation (parsing and built-in checking)
 * 2. Path resolution for the command
 * 3. Process creation and execution
 * 4. Error handling and memory cleanup
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

