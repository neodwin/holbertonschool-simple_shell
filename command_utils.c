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

	pid = fork();
	if (pid == -1)
	{
		free(cmd_path);
		perror("fork");
		return (1);
	}

	if (pid == 0)
		execute_command_child(cmd_path, args);
	else
	{
		waitpid(pid, &status, 0);
		free(cmd_path);
		if (WIFEXITED(status))
			return (WEXITSTATUS(status));
	}
	return (status);
}
