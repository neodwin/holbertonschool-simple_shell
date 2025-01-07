#include "shell.h"

/**
 * find_path_in_environ - Searches through environ array for PATH
 *
 * This function scans environment variables to find PATH variable.
 * If PATH is not found in environment, returns NULL.
 *
 * Return: Pointer to PATH value if found, NULL if not found
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
			return (*path == '\0' ? NULL : path);
		}
		i++;
	}
	return (NULL);
}

/**
 * check_current_path - Check if command exists in current path
 * @command: The command to check
 *
 * Return: Duplicated command if it contains '/', NULL otherwise
 */
char *check_current_path(char *command)
{
	if (command == NULL)
		return (NULL);

	if (strstr(command, "/") != NULL)
		return (strdup(command));

	return (NULL);
}

/**
 * try_path - Try to find command in given directory
 * @dir: The directory to check in
 * @command: The command name to find
 *
 * Return: Full path if command found, NULL otherwise
 */
char *try_path(char *dir, char *command)
{
	char *full_path;
	struct stat st;

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
 * check_absolute_path - Check if command is an absolute path
 * @command: Command to check
 *
 * Return: Duplicated command if valid, NULL otherwise
 */
char *check_absolute_path(char *command)
{
	struct stat st;

	if (stat(command, &st) == 0 && (st.st_mode & S_IXUSR))
		return (strdup(command));
	return (NULL);
}

/**
 * get_command_path - Get the full path of a command
 * @command: The command name to find
 *
 * Return: Full path to command if found, NULL if not found
 */
char *get_command_path(char *command)
{
	char *path, *path_copy, *dir;
	char *full_path;

	if (!command || !*command)
		return (NULL);

	if (strchr(command, '/') != NULL)
	{
		if (command[0] == '.')
			return (check_absolute_path(command));
		return (check_absolute_path(command));
	}

	path = find_path_in_environ();
	if (!path)
		return (try_path(".", command));

	path_copy = strdup(path);
	if (!path_copy)
		return (NULL);

	dir = strtok(path_copy, ":");
	while (dir)
	{
		full_path = try_path(dir, command);
		if (full_path)
		{
			free(path_copy);
			return (full_path);
		}
		dir = strtok(NULL, ":");
	}
	free(path_copy);

	return (try_path(".", command));
}
