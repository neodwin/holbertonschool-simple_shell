#include "shell.h"

/**
 * resolve_dots - Resolve dots in path
 * @path: Path to resolve
 *
 * Return: Resolved path (must be freed) or NULL
 */
char *resolve_dots(const char *path)
{
	char *resolved = strdup(path);
	char *token;
	char *result;
	int len = 0;

	if (!resolved)
		return (NULL);

	result = malloc(strlen(path) + 1);
	if (!result)
	{
		free(resolved);
		return (NULL);
	}
	result[0] = '\0';

	token = strtok(resolved, "/");
	while (token)
	{
		if (strcmp(token, ".") == 0)
			;
		else if (strcmp(token, "..") == 0)
			len = strlen(result) - 1;
		else
		{
			if (len > 0)
				strcat(result, "/");
			strcat(result, token);
			len = strlen(result);
		}
		token = strtok(NULL, "/");
	}

	free(resolved);
	return (result);
}

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

	if (command[0] == '/')
		cmd_path = strdup(command);
	else if (command[0] == '.')
		cmd_path = resolve_dots(command);
	else
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
	int i, arg_count;

	/* Count arguments */
	for (arg_count = 1; args[arg_count]; arg_count++)
		;

	/* Create new argument array */
	exec_args = malloc(sizeof(char *) * (arg_count + 1));
	if (!exec_args)
	{
		free(cmd_path);
		return (1);
	}

	/* Set up arguments */
	exec_args[0] = cmd_path;
	for (i = 2; i < arg_count; i++)
		exec_args[i - 1] = args[i];
	exec_args[arg_count - 1] = NULL;

	pid = fork();
	if (pid == -1)
	{
		free(cmd_path);
		free(exec_args);
		perror("fork");
		return (1);
	}

	if (pid == 0)
	{
		execve(cmd_path, exec_args, environ);
		perror("execve");
		free(cmd_path);
		free(exec_args);
		_exit(127);
	}

	waitpid(pid, &status, 0);
	free(cmd_path);
	free(exec_args);
	if (WIFEXITED(status))
		return (WEXITSTATUS(status));

	return (status);
}
