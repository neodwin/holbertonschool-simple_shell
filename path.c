#include "shell.h"

/**
 * find_path_in_environ - Searches through environ array for PATH
 *
 * This function scans environment variables to find PATH variable.
 * If PATH is not found in environment or is empty, returns NULL.
 * Only looks for exact "PATH=" match, ignores similar variables.
 *
 * Return: Pointer to PATH value if found and not empty, NULL otherwise
 */
char *find_path_in_environ(void)
{
	int i = 0;
	char *path = NULL;

	/* Return NULL if environ is NULL */
	if (!environ)
		return (NULL);

	/* Loop through environment variables until PATH is found */
	while (environ[i])
	{
		/* Check if current env variable is exactly "PATH=" */
		if (strcmp(environ[i], "PATH=") == 0)
			return (NULL);  /* PATH exists but is empty */

		if (strncmp(environ[i], "PATH=", 5) == 0)
		{
			path = environ[i] + 5;
			if (*path == '\0')
				return (NULL);  /* PATH exists but is empty */
			return (path);      /* Return valid PATH value */
		}
		i++;
	}
	return (NULL);  /* PATH not found */
}

/**
 * check_current_path - Check if command exists in current path
 * @command: The command to check
 *
 * This function checks if the command contains a slash, indicating
 * it's a path rather than just a command name. If it is a path,
 * duplicates the string for later use.
 *
 * Return: Duplicated command if it contains '/', NULL otherwise
 */
char *check_current_path(char *command)
{
	char *clean_path;
	int i;

	/* Return NULL if command is NULL */
	if (command == NULL)
		return (NULL);

	/* Check if command contains a path separator */
	if (strchr(command, '/') == NULL)
		return (NULL);

	/* Duplicate the command string */
	clean_path = strdup(command);
	if (!clean_path)
		return (NULL);

	/* Remove consecutive slashes and handle . and .. */
	i = 0;
	while (clean_path[i])
	{
		/* Skip multiple consecutive slashes */
		if (clean_path[i] == '/' && clean_path[i + 1] == '/')
		{
			memmove(&clean_path[i], &clean_path[i + 1], strlen(&clean_path[i]));
			continue;
		}
		i++;
	}

	return clean_path;
}

/**
 * try_path - Try to find command in given directory
 * @dir: The directory to check in
 * @command: The command name to find
 *
 * This function combines directory and command name to create a full path,
 * then checks if this path exists and is executable using stat.
 *
 * Return: Full path if command found, NULL otherwise
 */
char *try_path(char *dir, char *command)
{
	char *full_path;
	struct stat st;

	/* Allocate memory for full path (+2 for '/' and '\0') */
	full_path = malloc(strlen(dir) + strlen(command) + 2);
	if (!full_path)
		return (NULL);

	/* Combine dir and command to create full path */
	sprintf(full_path, "%s/%s", dir, command);

	/* Check if path exists and is accessible */
	if (stat(full_path, &st) == 0)
		return (full_path);

	/* Path not found, free memory and return NULL */
	free(full_path);
	return (NULL);
}

/**
 * get_command_path - Get the full path of a command
 * @command: The command name to find
 *
 * This function first checks if command contains a path, then searches
 * in PATH directories. Uses strtok to split PATH into individual directories.
 *
 * Return: Full path to command if found, NULL if not found
 */
char *get_command_path(char *command)
{
	char *path;
	char *path_copy;
	char *dir;
	char *full_path;

	/* First check if command is a path */
	full_path = check_current_path(command);
	if (full_path)
		return (full_path);

	/* Get PATH and check if it exists */
	path = find_path_in_environ();
	if (!path)
		return (NULL);

	/* Create a copy of PATH for modification */
	path_copy = malloc(strlen(path) + 1);
	if (!path_copy)
		return (NULL);
	strcpy(path_copy, path);

	/* Try each directory in PATH */
	dir = strtok(path_copy, ":");
	while (dir)
	{
		/* Try to find command in current directory */
		full_path = try_path(dir, command);
		if (full_path)
		{
			free(path_copy);
			return (full_path);
		}
		/* Move to next directory in PATH */
		dir = strtok(NULL, ":");
	}
	/* Command not found in any directory */
	free(path_copy);
	return (NULL);
}

/**
 * execute_builtin - Execute a command with its arguments
 * @command: The command to execute
 * @args: Array of command and its arguments
 *
 * Return: 0 on success, -1 on failure
 */
int execute_builtin(char *command, char **args)
{
	pid_t pid;
	char *cmd_path;
	int status;

	cmd_path = get_command_path(command);
	if (!cmd_path)
		return -1;

	pid = fork();
	if (pid == -1)
	{
		free(cmd_path);
		return -1;
	}

	if (pid == 0)
	{
		if (execve(cmd_path, args, environ) == -1)
		{
			free(cmd_path);
			exit(127);
		}
	}

	waitpid(pid, &status, 0);
	free(cmd_path);
	return WEXITSTATUS(status);
}
