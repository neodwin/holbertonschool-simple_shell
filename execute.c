#include "shell.h"

/**
 * execute_in_child - Execute a command in a child process
 * @cmd_path: Full path to the command executable
 * @args: Array of command arguments (including command name)
 * @program_name: Name of the shell program for error messages
 *
 * Description: Runs command via execve. On error: prints msg,
 * frees memory, exits 127.
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
 * Description: This function splits a command string
 * into an array of arguments using space, tab, and newline as delimiters.
 * It allocates memory for up to 32 arguments. Each argument is duplicated
 * in new memory. If any memory allocation fails, all previously allocated
 * memory is freed.
 *
 * Return: Array of strings (arguments) or NULL if memory allocation fails
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
 * Description: This function handles memory cleanup after command execution.
 * It frees both the command path string and the array of argument strings.
 * It checks for NULL pointers before attempting to free memory to prevent
 * segmentation faults.
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
 * Description: This function prepares a command for execution by:
 * 1. Removing leading whitespace
 * 2. Checking if the command is empty
 * 3. Parsing the command into arguments
 * 4. Checking if it's a built-in command
 * If it's a built-in command, it's handled here and NULL is returned.
 *
 * Return: Array of parsed arguments, or NULL if command is empty/built-in
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
 * Description: This is the main command execution function that:
 * 1. Prepares the command (parsing and built-in checking)
 * 2. Finds the full path of the command
 * 3. Creates a child process
 * 4. Executes the command in the child process
 * 5. Waits for the command to complete
 * 6. Handles any errors that occur during execution
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

