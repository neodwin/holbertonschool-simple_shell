#include "shell.h"

/**
 * try_path - Try to find command in specified directory
 * @dir: Directory path to search in
 * @command: Command name to look for
 * Description: Attempts to locate command in given directory.
 * Constructs full path, checks if file exists and is executable.
 * Allocates memory for path string if command is found.
 * Return: Full path to command if found, NULL otherwise
 */
char *try_path(const char *dir, const char *command)
{
/* Pointer to store full path */
char *full_path;
/* Lengths of directory and command */
size_t dir_len, cmd_len;
/* Structure to hold file status */
struct stat st;

	/* Calculate lengths of directory and command */
	dir_len = strlen(dir);
	cmd_len = strlen(command);

	/* Allocate memory for full path */
	full_path = malloc(dir_len + cmd_len + 2);
	if (!full_path)
		return (NULL); /* Return NULL if allocation fails */

	/* Copy directory to full path */
	strcpy(full_path, dir);

	/* Add '/' if not present at the end of directory */
	if (full_path[dir_len - 1] != '/')
	{
		full_path[dir_len] = '/';
		full_path[dir_len + 1] = '\0';
		dir_len++;
	}
	/* Append command to directory path */
	strcpy(full_path + dir_len, command);

	/* Check if file exists and is executable */
	if (stat(full_path, &st) == 0 && (st.st_mode & S_IXUSR))
		return (full_path); /* Return full path if executable */

	/* Free allocated memory if not found */
	free(full_path);
	return (NULL);
}

/**
 * find_path_in_env - Find PATH variable in environment
 * Description: Searches through environment variables to find
 * PATH. Returns pointer to value part of PATH=value string.
 * Handles case where PATH is not set or is empty.
 * Return: Pointer to PATH value, NULL if not found
 */
char *find_path_in_env(void)
{
	char **env; /* Pointer to traverse environment variables */
	const char *path_prefix = "PATH="; /* Prefix to identify PATH */
	size_t prefix_len = strlen(path_prefix); /* Length of prefix */

	/* Iterate over environment variables */
	for (env = environ; *env; env++)
	{
		/* Check if current variable starts with "PATH=" */
		if (strncmp(*env, path_prefix, prefix_len) == 0)
			return (*env + prefix_len); /* Return pointer to PATH value */
	}
	return (NULL); /* Return NULL if PATH not found */
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
 * Return: Full path to command if found, NULL otherwise
 */
char *get_path(char *command)
{
	/* Pointers for path handling */
	char *path_env, *path_copy, *dir, *result;
	/* Structure to hold file status */
	struct stat st;
	/* Check if command contains a path separator */
	if (strchr(command, '/'))
	{
		/* Check if command is executable */
		if (stat(command, &st) == 0 && (st.st_mode & S_IXUSR))
			return (strdup(command)); /* Return duplicate of command */
		return (NULL); /* Return NULL if not executable */
	}
	/* Check if command is executable in current directory */
	if (stat(command, &st) == 0 && (st.st_mode & S_IXUSR))
		return (strdup(command)); /* Return duplicate of command */
	/* Find PATH environment variable */
	path_env = find_path_in_env();
	if (!path_env)
		return (NULL); /* Return NULL if PATH not found */
	/* Duplicate PATH value for tokenization */
	path_copy = strdup(path_env);
	if (!path_copy)
		return (NULL); /* Return NULL if duplication fails */
	/* Tokenize PATH and search for command */
	dir = strtok(path_copy, ":");
	while (dir)
	{
		/* Try to find command in current directory */
		result = try_path(dir, command);
		if (result)
		{
			free(path_copy); /* Free duplicated PATH */
			return (result); /* Return full path if found */
		}
		dir = strtok(NULL, ":"); /* Move to next directory */
	}
	/* Free duplicated PATH if command not found */
	free(path_copy);
	return (NULL);
}
