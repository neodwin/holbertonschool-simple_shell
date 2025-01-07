#include "shell.h"

/**
 * find_path_in_environ - Searches through environ array for PATH
 *
 * This function scans environment variables to find PATH variable.
 * If PATH is not found in environment, returns default path.
 *
 * Return: Pointer to PATH value if found, default path if not found
 */
char *find_path_in_environ(void)
{
	int i = 0;
	char *path = NULL;

	while (environ[i])
	{
		if (strncmp(environ[i], "PATH=", 5) == 0)
		{
			path = environ[i] + 5;
			return (*path == '\0' ? "/bin:/usr/bin" : path);
		}
		i++;
	}
	return ("/bin:/usr/bin");  /* Default PATH if not found */
}

/**
 * check_current_path - Check if command exists in current path
 * @command: The command to check
 *
 * Return: Duplicated command if it exists, NULL otherwise
 */
char *check_current_path(char *command)
{
	struct stat st;

	if (command == NULL)
		return (NULL);

	if (stat(command, &st) == 0 && (st.st_mode & S_IXUSR))
		return (strdup(command));

	return (NULL);
}

/**
 * try_path - Try to find command in given directory
 * @dir: The directory to check in
 * @command: The command name to find
 *
 * Return: Full path if command found and executable, NULL otherwise
 */
char *try_path(char *dir, char *command)
{
	char *full_path;
	struct stat st;

	if (!dir || !command)
		return (NULL);

	full_path = malloc(strlen(dir) + strlen(command) + 2);
	if (!full_path)
		return (NULL);

	sprintf(full_path, "%s/%s", dir, command);

	if (stat(full_path, &st) == 0 && (st.st_mode & S_IXUSR))
		return (full_path);

	free(full_path);
	return (NULL);
}

/**
 * get_next_path - Get next path from PATH string
 * @path_str: PATH string
 * @start: Starting position
 * @len: Length of next path
 *
 * Return: Pointer to next path, NULL if no more paths
 */
char *get_next_path(char *path_str, char **start, size_t *len)
{
	char *end;
	char *path;

	if (!path_str || !*start || !**start)
		return (NULL);

	end = strchr(*start, ':');
	if (!end)
	{
		*len = strlen(*start);
		path = *start;
		*start = *start + *len;
		return (path);
	}

	*len = end - *start;
	path = *start;
	*start = end + 1;
	return (path);
}

/**
 * get_command_path - Get the full path of a command
 * @command: The command name to find
 *
 * Return: Full path to command if found and executable, NULL if not found
 */
char *get_command_path(char *command)
{
	char *path, *path_start, *dir;
	char *full_path = NULL;
	size_t dir_len;
	char *temp_path;

	if (!command || !*command)
		return (NULL);

	/* First check if command contains a path */
	if (strchr(command, '/'))
		return (check_current_path(command));

	/* Get PATH environment variable */
	path = find_path_in_environ();
	if (!path)
		return (NULL);

	/* Try each directory in PATH */
	path_start = path;
	while ((dir = get_next_path(path, &path_start, &dir_len)))
	{
		temp_path = malloc(dir_len + 1);
		if (!temp_path)
			return (NULL);

		strncpy(temp_path, dir, dir_len);
		temp_path[dir_len] = '\0';

		full_path = try_path(temp_path, command);
		free(temp_path);

		if (full_path)
			return (full_path);
	}

	return (NULL);
}
