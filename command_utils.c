#include "shell.h"

/**
 * get_command_path_ext - Get command path with extended checks
 * @command: Command to check
 *
 * Return: Command path (must be freed) or NULL
 */
char *get_command_path_ext(const char *command)
{
	char *cmd_path;
	struct stat st;

	if (!command || !*command)
		return (NULL);

	if (command[0] == '/')
		return (check_absolute_path(command));

	if (command[0] == '.')
		return (check_relative_path(command));

	cmd_path = get_command_path((char *)command);
	if (!cmd_path)
		return (NULL);

	if (stat(cmd_path, &st) != 0 || !(st.st_mode & S_IXUSR))
	{
		free(cmd_path);
		return (NULL);
	}

	return (cmd_path);
}

/**
 * setup_exec_args - Set up execution arguments
 * @args: Original arguments
 * @cmd_path: Command path
 * @arg_count: Number of arguments
 *
 * Return: Array of execution arguments or NULL on failure
 */
char **setup_exec_args(char **args, char *cmd_path, int arg_count)
{
	char **exec_args;
	int i;

	exec_args = malloc(sizeof(char *) * (arg_count + 1));
	if (!exec_args)
		return (NULL);

	exec_args[0] = cmd_path;
	for (i = 1; i < arg_count; i++)
		exec_args[i] = args[i];
	exec_args[arg_count] = NULL;

	return (exec_args);
}

/**
 * execute_in_child - Execute command in child process
 * @cmd_path: Command path
 * @exec_args: Execution arguments
 *
 * Return: Does not return on success, exits on failure
 */
void execute_in_child(char *cmd_path, char **exec_args)
{
	execve(cmd_path, exec_args, environ);
	perror("execve");
	free(cmd_path);
	free(exec_args);
	_exit(127);
}

/**
 * execute_command_ext - Execute command with error handling
 * @cmd_path: Command path
 * @args: Command arguments
 *
 * Return: Exit status
 */
int execute_command_ext(char *cmd_path, char **args)
{
	pid_t pid;
	int status = 0;
	char **exec_args;
	int arg_count;

	if (!cmd_path)
		return (127);

	/* Count arguments */
	for (arg_count = 1; args[arg_count]; arg_count++)
		;

	exec_args = setup_exec_args(args, cmd_path, arg_count);
	if (!exec_args)
	{
		free(cmd_path);
		return (1);
	}

	pid = fork();
	if (pid == -1)
	{
		free(cmd_path);
		free(exec_args);
		perror("fork");
		return (1);
	}

	if (pid == 0)
		execute_in_child(cmd_path, exec_args);

	waitpid(pid, &status, 0);
	free(cmd_path);
	free(exec_args);
	if (WIFEXITED(status))
		return (WEXITSTATUS(status));

	return (status);
}
