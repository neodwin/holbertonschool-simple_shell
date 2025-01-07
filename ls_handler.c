#include "shell.h"

/**
 * handle_ls_path - Get path for ls command
 * @command: Command string (unused)
 *
 * Return: Path to ls executable (must be freed) or NULL
 */
char *handle_ls_path(const char *command)
{
	char path[PATH_MAX];
	char *normalized;
	(void)command;

	if (!setup_ls_path(path))
		return (NULL);

	normalized = normalize_path(path);
	if (!normalized)
		return (NULL);

	return (normalized);
}

/**
 * setup_ls_args - Set up arguments for ls command
 * @command: Original command string
 * @args: Array to store arguments
 *
 * Return: Number of arguments
 */
int setup_ls_args(const char *command, char **args)
{
	int i = 0;
	char *token;
	char *cmd_copy = strdup(command);

	if (!cmd_copy)
		return (0);

	token = strtok(cmd_copy, " \t");
	while (token && i < 63)
	{
		args[i] = strdup(token);
		if (!args[i])
		{
			while (--i >= 0)
				free(args[i]);
			free(cmd_copy);
			return (0);
		}
		i++;
		token = strtok(NULL, " \t");
	}
	args[i] = NULL;

	free(cmd_copy);
	return (i);
}

/**
 * cleanup_ls_args - Clean up ls arguments
 * @args: Array of arguments
 */
void cleanup_ls_args(char **args)
{
	int i;

	for (i = 0; args[i]; i++)
		free(args[i]);
}

/**
 * execute_ls - Execute ls command
 * @command: Command string
 * @shell_args: Shell arguments
 *
 * Return: Exit status
 */
int execute_ls(char *command, char **shell_args)
{
	char *ls_path;
	char *ls_args[64];
	int arg_count;
	pid_t pid;
	int status;

	ls_path = handle_ls_path(command);
	if (!ls_path)
		return (1);

	arg_count = setup_ls_args(command, ls_args);
	if (arg_count == 0)
	{
		free(ls_path);
		return (1);
	}

	pid = fork();
	if (pid == -1)
	{
		cleanup_ls_args(ls_args);
		free(ls_path);
		return (1);
	}

	if (pid == 0)
	{
		execve(ls_path, ls_args, environ);
		perror(shell_args[0]);
		cleanup_ls_args(ls_args);
		free(ls_path);
		_exit(127);
	}

	waitpid(pid, &status, 0);
	cleanup_ls_args(ls_args);
	free(ls_path);

	return (WIFEXITED(status) ? WEXITSTATUS(status) : 1);
}
