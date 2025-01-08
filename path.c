#include "shell.h"

/**
 * try_path - Try to find command in a directory
 * @dir: Directory to search in
 * @command: Command to find
 * @program_name: Program name
 * Return: Full path if found, NULL otherwise
 */
char *try_path(const char *dir, const char *command, char *program_name)
{
	char *cmd_path;
	struct stat st;

	cmd_path = malloc(strlen(dir) + strlen(command) + 2);
	if (!cmd_path)
		return (NULL);

	sprintf(cmd_path, "%s/%s", dir, command);
	if (stat(cmd_path, &st) == 0)
	{
		if (!(st.st_mode & S_IXUSR))
		{
			fprintf(stderr, "%s: 1: %s: Permission denied\n",
				program_name, command);
			free(cmd_path);
			exit(126);
		}
		if (S_ISREG(st.st_mode))
			return (cmd_path);
	}

	free(cmd_path);
	return (NULL);
}

/**
 * find_path_in_env - Find PATH in environment variables
 * Return: PATH value or NULL if not found
 */
char *find_path_in_env(void)
{
	int i;

	if (!environ)
		return ("/bin");

	for (i = 0; environ[i]; i++)
	{
		if (strncmp(environ[i], "PATH=", 5) == 0)
			return (environ[i] + 5);
	}
	return ("/bin");
}

/**
 * get_path - Get the full path of a command
 * @command: Command to find
 * @program_name: Program name
 * Return: Full path of command if found, NULL otherwise
 */
char *get_path(char *command, char *program_name)
{
	char *path_env, *path_copy, *dir, *cmd_path;
	struct stat st;

	if (!command)
		return (NULL);

	if (command[0] == '/' || command[0] == '.' ||
	    strstr(command, "..") != NULL)
	{
		if (stat(command, &st) == 0)
		{
			if (!(st.st_mode & S_IXUSR))
			{
				fprintf(stderr, "%s: 1: %s: Permission denied\n",
					program_name, command);
				exit(126);
			}
			if (S_ISREG(st.st_mode))
				return (strdup(command));
		}
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
		cmd_path = try_path(dir, command, program_name);
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
