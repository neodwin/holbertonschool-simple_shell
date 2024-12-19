#include "shell.h"

/**
 * find_path_in_environ - Find PATH in environment variables
 * Return: PATH value or default path if not found
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
			break;
		}
		i++;
	}
	return (path ? path : "/bin:/usr/bin");
}

**
 * get_command_path - Get the full path of a command
 * Description: Searches for a command in PATH directories
 * First checks if command exists as is, then searches in PATH
 *
 * @command: Name of the command to find
 * Return: Full path to command if found, NULL if not found
 */
char *get_command_path(char *command)

7:25

	char *path;         /* Original PATH string */
	char *path_copy;    /* Copy of PATH to modify */
	char *dir;          /* Current directory being checked */
	char *full_path;    /* Complete path to command */
	int cmd_len, dir_len;  /* Length of command and directory */

7:26

/* Check if command exists in current directory */
	if (access(command, X_OK) == 0)
		return (strdup(command));

	/* Get PATH and prepare for scanning */
	path = find_path_in_environ();
	cmd_len = strlen(command);
	path_copy = malloc(strlen(path) + 1);
	if (!path_copy)
		return (NULL);
	strcpy(path_copy, path);