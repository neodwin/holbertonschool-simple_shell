#include "shell.h"

/**
 * normalize_path - Normalize path by removing extra slashes
 * @path: Path to normalize
 *
 * Return: Normalized path (must be freed) or NULL
 */
char *normalize_path(const char *path)
{
	char *normalized = malloc(strlen(path) + 1);
	int i = 0, j = 0;

	if (!normalized)
		return (NULL);

	while (path[i])
	{
		if (path[i] == '/' && (i == 0 || path[i - 1] != '/'))
			normalized[j++] = path[i];
		else if (path[i] != '/')
			normalized[j++] = path[i];
		i++;
	}
	normalized[j] = '\0';

	return (normalized);
}

/**
 * check_ls_path - Check if path exists and is executable
 * @path: Path to check
 *
 * Return: 1 if valid, 0 otherwise
 */
int check_ls_path(const char *path)
{
	struct stat st;

	return (stat(path, &st) == 0 && (st.st_mode & S_IXUSR));
}

/**
 * is_ls_command - Check if command is ls
 * @command: Command to check
 *
 * Return: 1 if ls command, 0 otherwise
 */
int is_ls_command(const char *command)
{
	const char *cmd = command;

	while (*cmd == ' ' || *cmd == '\t')
		cmd++;

	return (strcmp(cmd, "ls") == 0 || strncmp(cmd, "ls ", 3) == 0);
}

/**
 * setup_ls_path - Set up ls path
 * @path: Buffer to store path
 *
 * Return: 1 on success, 0 on failure
 */
int setup_ls_path(char *path)
{
	if (getcwd(path, PATH_MAX) == NULL)
		return (0);

	strcat(path, "/ls");
	if (check_ls_path(path))
		return (1);

	strcpy(path, "/bin/ls");
	if (check_ls_path(path))
		return (1);

	return (0);
}
