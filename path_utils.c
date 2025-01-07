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
	char *result;
	char cwd[PATH_MAX];
	char *token;
	char *saveptr;
	char *path_copy;
	char *temp;

	if (!getcwd(cwd, sizeof(cwd)))
		return (NULL);

	fprintf(stderr, "DEBUG: CWD = %s\n", cwd);
	fprintf(stderr, "DEBUG: Input path = %s\n", path);

	/* If path starts with /, use it as is */
	if (path[0] == '/')
	{
		result = strdup(path);
		if (!result)
			return (NULL);
		return (result);
	}

	/* For relative paths, start from current directory */
	result = strdup(cwd);
	if (!result)
		return (NULL);

	path_copy = strdup(path);
	if (!path_copy)
	{
		free(result);
		return (NULL);
	}

	token = strtok_r(path_copy, "/", &saveptr);
	while (token)
	{
		fprintf(stderr, "DEBUG: Processing token = %s\n", token);
		if (strcmp(token, "..") == 0)
		{
			char *last_slash = strrchr(result, '/');
			if (last_slash && last_slash != result)
			{
				*last_slash = '\0';
				fprintf(stderr, "DEBUG: After .. = %s\n", result);
			}
		}
		else if (strcmp(token, ".") != 0)
		{
			temp = malloc(strlen(result) + strlen(token) + 2);
			if (!temp)
			{
				free(result);
				free(path_copy);
				return (NULL);
			}
			sprintf(temp, "%s/%s", result, token);
			free(result);
			result = temp;
			fprintf(stderr, "DEBUG: After token = %s\n", result);
		}
		token = strtok_r(NULL, "/", &saveptr);
	}

	fprintf(stderr, "DEBUG: Final path = %s\n", result);
	free(path_copy);
	return (result);
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
	char *cmd_path;
	struct stat st;

	if (command[0] == '.' && command[1] == '/')
	{
		if (!getcwd(cwd, sizeof(cwd)))
			return (NULL);

		cmd_path = resolve_dots(command);
		if (!cmd_path)
			return (NULL);

		if (stat(cmd_path, &st) == 0 && (st.st_mode & S_IXUSR))
			return (cmd_path);

		free(cmd_path);
		return (NULL);
	}

	/* Handle other relative paths */
	cmd_path = resolve_dots(command);
	if (!cmd_path)
		return (NULL);

	if (stat(cmd_path, &st) == 0 && (st.st_mode & S_IXUSR))
		return (cmd_path);

	free(cmd_path);
	return (NULL);
}
