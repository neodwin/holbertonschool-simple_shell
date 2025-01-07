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
		fprintf(stderr, "%s: 1: %s: not found\n",
			program_name, args[0]);
		cleanup(cmd_path, args);
		exit(127);
	}
}

/**
 * execute_command_helper - Helper function to execute command
 * @cmd_path: Path to command
 * @args: Command arguments
 * @program_name: Name of shell program
 */
void execute_command_helper(char *cmd_path, char **args, char *program_name)
{
	pid_t pid;
	int status;

	pid = fork();
	if (pid == -1)
	{
		perror("fork");
		cleanup(cmd_path, args);
		return;
	}

	if (pid == 0)
	{
		if (execve(cmd_path, args, environ) == -1)
		{
			fprintf(stderr, "%s: 1: %s: not found\n",
				program_name, args[0]);
			cleanup(cmd_path, args);
			exit(127);
		}
	}
	else
	{
		waitpid(pid, &status, 0);
		cleanup(cmd_path, args);
	}
}

/**
 * execute_command - Execute a command
 * @command: Command to execute
 * @program_name: Name of the shell program
 */
void execute_command(char *command, char *program_name)
{
	char **args;
	char *cmd_path = NULL;
	char *trimmed_command = prepare_command(command);

	if (*trimmed_command == '\0')
		return;

	args = parse_command(trimmed_command);
	if (!args)
		return;

	if (handle_builtin(args))
	{
		cleanup(NULL, args);
		return;
	}

	cmd_path = get_path(args[0]);
	if (!cmd_path)
	{
		fprintf(stderr, "%s: 1: %s: not found\n",
			program_name, args[0]);
		cleanup(cmd_path, args);
		return;
	}

	execute_command_helper(cmd_path, args, program_name);
}

