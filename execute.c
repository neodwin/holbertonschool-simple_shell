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
	/* Attempt to execute the command */
	if (execve(cmd_path, args, environ) == -1)
	{
		/* Print error message if execution fails */
		fprintf(stderr, "%s: 1: %s: not found\n", program_name,
			args[0]);
		/* Free command path memory */
		free(cmd_path);
		/* Exit with command not found status */
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
	/* Allocate memory for argument pointers */
	char **args = malloc(sizeof(char *) * 32);
	/* Token for string splitting */
	char *token;
	/* Counter for number of arguments */
	int i = 0;

	/* Check allocation success */
	if (!args)
		return (NULL);

	/* Get first token */
	token = strtok(command, " \t\n");
	/* Process up to 31 arguments */
	while (token && i < 31)
	{
		/* Duplicate token into new memory */
		args[i] = strdup(token);
		/* Check duplication success */
		if (!args[i])
		{
			/* Free all previously allocated memory */
			while (--i >= 0)
				free(args[i]);
			free(args);
			return (NULL);
		}
		/* Move to next argument */
		i++;
		/* Get next token */
		token = strtok(NULL, " \t\n");
	}
	/* Null-terminate argument array */
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
	/* Counter for argument array */
	int i;

	/* Free command path if not NULL */
	if (cmd_path)
		free(cmd_path);
	/* Free argument array if not NULL */
	if (args)
	{
		/* Free each argument string */
		for (i = 0; args[i]; i++)
			free(args[i]);
		/* Free argument array */
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
	/* Array for parsed arguments */
	char **args;

	/* Skip leading whitespace */
	while (*command == ' ' || *command == '\t')
		command++;

	/* Check if command is empty */
	if (*command == '\0')
		return (NULL);

	/* Parse command into arguments */
	args = parse_command(command);
	/* Check parsing success */
	if (!args)
		return (NULL);

	/* Check if command is built-in */
	if (handle_builtin(args))
	{
		/* Clean up and return NULL for built-ins */
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
	/* Process ID for forking */
	pid_t pid;
	/* Status for wait */
	int status;
	/* Array of command arguments */
	char **args;
	/* Full path to command executable */
	char *cmd_path = NULL;
	/* Prepare command for execution */
	args = prepare_command(command);
	if (!args)
		return;
	/* Get full path of command */
	cmd_path = get_path(args[0]);
	if (!cmd_path)
	{
		/* Print error if command not found */
		fprintf(stderr, "%s: 1: %s: not found\n",
			program_name, args[0]);
		cleanup(cmd_path, args);
		return;
	}
	/* Create child process */
	pid = fork();
	if (pid == -1)
	{
		/* Handle fork error */
		perror("fork");
		cleanup(cmd_path, args);
		return;
	}
	/* Execute command in child process */
	if (pid == 0)
		execute_in_child(cmd_path, args, program_name);

	/* Wait for child process to complete */
	waitpid(pid, &status, 0);
	/* Clean up allocated memory */
	cleanup(cmd_path, args);
}

