#include "shell.h"

/**
 * try_path - Try to find command in specified directory
 * @dir: Directory path to search in
 * @command: Command name to look for
 *
 * Description: Attempts to locate command in given directory.
 * Constructs full path, checks if file exists and is executable.
 * Allocates memory for path string if command is found.
 *
 * Return: Full path to command if found, NULL otherwise
 */
char *try_path(const char *dir, const char *command)
{
	char *full_path;
	size_t dir_len, cmd_len;
	struct stat st;

	dir_len = strlen(dir);
	cmd_len = strlen(command);
	full_path = malloc(dir_len + cmd_len + 2);
	if (!full_path)
		return (NULL);

	strcpy(full_path, dir);
	if (full_path[dir_len - 1] != '/')
	{
		full_path[dir_len] = '/';
		full_path[dir_len + 1] = '\0';
		dir_len++;
	}
	strcpy(full_path + dir_len, command);

	if (stat(full_path, &st) == 0 && (st.st_mode & S_IXUSR))
		return (full_path);

	free(full_path);
	return (NULL);
}

/**
 * find_path_in_env - Find PATH variable in environment
 *
 * Description: Searches through environment variables to find
 * PATH. Returns pointer to value part of PATH=value string.
 * Handles case where PATH is not set or is empty.
 *
 * Return: Pointer to PATH value, NULL if not found
 */
char *find_path_in_env(void)
{
	char **env;
	const char *path_prefix = "PATH=";
	size_t prefix_len = strlen(path_prefix);

	for (env = environ; *env; env++)
	{
		if (strncmp(*env, path_prefix, prefix_len) == 0)
			return (*env + prefix_len);
	}
	return (NULL);
}

/**
 * get_path - Get full path for a command
 * @command: Command name to resolve
 *
 * Description: Resolves full path for command by:
 * 1. Checking if command contains path separator
 * 2. Checking if command exists in current directory
 * 3. Searching in PATH directories
 * Handles memory allocation and path construction.
 *
 * Return: Full path to command if found, NULL otherwise
 */
char *get_path(char *command)
{
	char *path_env, *path_copy, *dir, *result;
	struct stat st;

	if (strchr(command, '/'))
	{
		if (stat(command, &st) == 0 && (st.st_mode & S_IXUSR))
			return (strdup(command));
		return (NULL);
	}

	if (stat(command, &st) == 0 && (st.st_mode & S_IXUSR))
		return (strdup(command));

	path_env = find_path_in_env();
	if (!path_env)
		return (NULL);

	path_copy = strdup(path_env);
	if (!path_copy)
		return (NULL);

	dir = strtok(path_copy, ":");
	while (dir)
	{
		result = try_path(dir, command);
		if (result)
		{
			free(path_copy);
			return (result);
		}
		dir = strtok(NULL, ":");
	}

	free(path_copy);
	return (NULL);
}
