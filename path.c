#include "shell.h"

/**
 * try_path - Attempt to find a command in a specific directory
 * @dir: Directory to search in
 * @command: Command name to look for
 *
 * Description: This function constructs a full path by combining a directory
 * and command name, then checks if that path exists and is executable.
 * It allocates memory for the full path and performs proper error checking
 * on memory allocation and file status.
 *
 * Return: Full path to command if found and executable, NULL otherwise
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
 * find_path_in_env - Find the PATH variable in environment
 *
 * Description: This function searches through the environment variables
 * to find the PATH variable. If found, it returns a pointer to the value
 * portion of PATH (after "PATH="). If PATH is not found in the environment,
 * it returns NULL, ensuring the shell behaves correctly in minimal
 * environments.
 *
 * Return: Pointer to PATH value if found, NULL if not found
 */
char *find_path_in_env(void)
{
	int i;

	for (i = 0; environ[i]; i++)
	{
		if (strncmp(environ[i], "PATH=", 5) == 0)
			return (environ[i] + 5);
	}
	return (NULL);
}

/**
 * get_path - Get the full path of a command
 * @command: Command name to find
 *
 * Description: This function finds the full path of a command by:
 * 1. Checking if it's already a path (starts with / or .)
 * 2. Looking in PATH directories if it's a bare command
 * The function handles:
 * - Absolute paths (/bin/ls)
 * - Relative paths (./script)
 * - Commands in PATH (ls)
 * - Memory allocation and cleanup
 *
 * Return: Full path to command if found, NULL if not found or on error
 */
char *get_path(char *command)
{
	char *path_env, *path_copy, *dir, *cmd_path;
	struct stat st;

	if (!command)
		return (NULL);

	if (command[0] == '/' || command[0] == '.' ||
	    strstr(command, "..") != NULL)
	{
		if (stat(command, &st) == 0 && (st.st_mode & S_IXUSR))
			return (strdup(command));
		return (NULL);
	}

	path_env = find_path_in_env();
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
