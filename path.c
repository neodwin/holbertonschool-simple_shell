#include "shell.h"

/**
 * normalize_path - Normalize a path by removing extra slashes and handling ..
 * @path: Path to normalize
 * Return: Normalized path (must be freed) or NULL
 */
char *normalize_path(const char *path)
{
	char *norm_path, *current;
	size_t len;
	struct stat st;

	if (!path)
		return (NULL);

	len = strlen(path) + 1;
	norm_path = malloc(len);
	if (!norm_path)
		return (NULL);

	strcpy(norm_path, path);
	current = norm_path;

	/* Remove extra slashes */
	while (*current)
	{
		if (*current == '/' && *(current + 1) == '/')
		{
			memmove(current, current + 1, strlen(current));
			continue;
		}
		current++;
	}

	/* Check if file exists and is executable */
	if (stat(norm_path, &st) == 0 && (st.st_mode & S_IXUSR))
		return (norm_path);

	free(norm_path);
	return (NULL);
}

/**
 * try_path - Try to find command in a specific directory
 * @dir: Directory to search in
 * @command: Command to find
 * Return: Full path if found and executable, NULL otherwise
 */
char *try_path(const char *dir, const char *command)
{
	char *full_path;
	size_t dir_len, cmd_len;
	struct stat st;

	if (!dir || !command)
		return (NULL);

	dir_len = strlen(dir);
	cmd_len = strlen(command);
	full_path = malloc(dir_len + cmd_len + 2);
	if (!full_path)
		return (NULL);

	strcpy(full_path, dir);
	if (dir_len > 0 && dir[dir_len - 1] != '/')
		strcat(full_path, "/");
	strcat(full_path, command);

	if (stat(full_path, &st) == 0 && (st.st_mode & S_IXUSR))
		return (full_path);

	free(full_path);
	return (NULL);
}

/**
 * find_path_in_env - Find PATH in environment variables
 * @env: Environment variables array
 * Return: PATH value or NULL if not found
 */
char *find_path_in_env(char **env)
{
	int i;
	const char *prefix = "PATH=";
	size_t prefix_len = strlen(prefix);

	if (!env)
		return (NULL);

	for (i = 0; env[i]; i++)
	{
		if (strncmp(env[i], prefix, prefix_len) == 0)
			return (env[i] + prefix_len);
	}
	return (NULL);
}

/**
 * get_path - Get full path for a command
 * @command: Command to find
 * @env: Environment variables
 * Return: Full path if found, NULL otherwise
 */
char *get_path(const char *command, char **env)
{
	char *path_env, *path_copy = NULL, *dir, *result = NULL;
	struct stat st;

	if (!command || !command[0])
		return (NULL);

	/* Handle absolute path */
	if (command[0] == '/')
	{
		if (stat(command, &st) == 0 && (st.st_mode & S_IXUSR))
			return (strdup(command));
		return (NULL);
	}

	/* Handle relative path */
	if (command[0] == '.' || strstr(command, "/"))
		return (normalize_path(command));

	/* Search in PATH */
	path_env = find_path_in_env(env);
	if (!path_env)
		return (NULL);

	path_copy = strdup(path_env);
	if (!path_copy)
		return (NULL);

	dir = strtok(path_copy, ":");
	while (dir && !result)
	{
		result = try_path(dir, command);
		dir = strtok(NULL, ":");
	}

	free(path_copy);
	return (result);
}
