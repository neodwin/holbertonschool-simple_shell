#include "shell.h"

/**
 * try_path - Try to find command in a directory
 * @dir: Directory to search in
 * @command: Command to find
 * Return: Full path if found, NULL otherwise
 */
char *try_path(const char *dir, const char *command)
{
	char *cmd_path;
	struct stat st;

	cmd_path = malloc(strlen(dir) + strlen(command) + 2);
	if (!cmd_path)
		return (NULL);

	sprintf(cmd_path, "%s/%s", dir, command);
	if (stat(cmd_path, &st) == 0 && (st.st_mode & S_IXUSR))
		return (cmd_path);

	free(cmd_path);
	return (NULL);
}

/**
 * get_path - Get the full path of a command
 * @command: Command to find
 * Return: Full path of command if found, NULL otherwise
 */
char *get_path(char *command)
{
	char *path_env, *path_copy, *dir, *cmd_path;
	struct stat st;

	if (!command)
		return (NULL);

	if (command[0] == '/' || command[0] == '.')
	{
		if (stat(command, &st) == 0 && (st.st_mode & S_IXUSR))
			return (strdup(command));
		return (NULL);
	}

	path_env = getenv("PATH");
	if (!path_env)
		return (NULL);

	path_copy = strdup(path_env);
	if (!path_copy)
		return (NULL);

	dir = strtok(path_copy, ":");
	while (dir)
	{
		cmd_path = try_path(dir, command);
		if (cmd_path)
		{
			free(path_copy);
			return (cmd_path);
		}
		dir = strtok(NULL, ":");
	}
	free(path_copy);
	return (NULL);
}
