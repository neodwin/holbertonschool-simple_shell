#include "shell.h"

/**
 * init_path_resolution - Initialize path resolution
 * @path: Original path
 * @cwd: Buffer for current working directory
 * @result: Pointer to result string
 *
 * Return: Resolved path or NULL on failure
 */
char *init_path_resolution(const char *path, char *cwd, char **result)
{
	char *resolved = strdup(path);

	if (!resolved || !getcwd(cwd, PATH_MAX))
	{
		free(resolved);
		return (NULL);
	}

	*result = strdup(cwd);
	if (!*result)
	{
		free(resolved);
		return (NULL);
	}

	return (resolved);
}

/**
 * process_path_token - Process a single path token
 * @token: Path token to process
 * @result: Current path result
 *
 * Return: 0 on success, -1 on failure
 */
int process_path_token(const char *token, char *result)
{
	char *last_slash;

	if (strcmp(token, "..") == 0)
	{
		last_slash = strrchr(result, '/');
		if (last_slash && last_slash != result)
			*last_slash = '\0';
	}
	else if (strcmp(token, ".") != 0)
	{
		strcat(result, "/");
		strcat(result, token);
	}
	return (0);
}

/**
 * resolve_dots - Resolve dots in path
 * @path: Path to resolve
 *
 * Return: Resolved path (must be freed) or NULL
 */
char *resolve_dots(const char *path)
{
	char *resolved;
	char *result;
	char cwd[PATH_MAX];
	char *path_copy;
	char *final_path;
	char *token;

	resolved = init_path_resolution(path, cwd, &result);
	if (!resolved)
		return (NULL);

	path_copy = strdup(path);
	if (!path_copy)
	{
		free(resolved);
		free(result);
		return (NULL);
	}

	token = strtok(path_copy, "/");
	while (token)
	{
		if (process_path_token(token, result) != 0)
			break;
		token = strtok(NULL, "/");
	}

	final_path = strdup(result);
	free(resolved);
	free(result);
	free(path_copy);
	return (final_path);
}

/**
 * check_absolute_path - Check if absolute path exists and is executable
 * @command: Command to check
 *
 * Return: Duplicated command path if valid, NULL otherwise
 */
char *check_absolute_path(const char *command)
{
	struct stat st;

	if (stat(command, &st) == 0 && (st.st_mode & S_IXUSR))
		return (strdup(command));
	return (NULL);
}

/**
 * check_relative_path - Check if relative path exists and is executable
 * @command: Command to check
 *
 * Return: Full path if valid, NULL otherwise
 */
char *check_relative_path(const char *command)
{
	char cwd[PATH_MAX];
	char *full_path;
	char *cmd_path;
	struct stat st;

	if (!getcwd(cwd, sizeof(cwd)))
		return (NULL);

	full_path = malloc(PATH_MAX);
	if (!full_path)
		return (NULL);

	snprintf(full_path, PATH_MAX, "%s/%s", cwd, command);
	cmd_path = resolve_dots(full_path);
	free(full_path);

	if (cmd_path && stat(cmd_path, &st) == 0 && (st.st_mode & S_IXUSR))
		return (cmd_path);

	free(cmd_path);
	return (NULL);
}
