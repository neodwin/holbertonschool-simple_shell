#include "shell.h"

/**
 * resolve_path_dots - Helper function for resolve_dots
 * @path: Path to resolve
 * @cwd: Current working directory (unused)
 * @result: Result buffer
 *
 * Return: Resolved path or NULL on failure
 */
char *resolve_path_dots(const char *path, char *cwd, char *result)
{
	char *path_copy;
	char *token;
	char *saveptr;
	char *last_slash;

	(void)cwd;  /* Mark cwd as used */

	path_copy = strdup(path);
	if (!path_copy)
		return (NULL);

	token = strtok_r(path_copy, "/", &saveptr);
	while (token)
	{
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
		token = strtok_r(NULL, "/", &saveptr);
	}

	free(path_copy);
	return (result);
}

/**
 * resolve_dots - Resolve dots in path
 * @path: Path to resolve
 *
 * Return: Resolved path (must be freed) or NULL
 */
char *resolve_dots(const char *path)
{
	char cwd[PATH_MAX];
	char *result;

	if (!getcwd(cwd, sizeof(cwd)))
		return (NULL);

	if (path[0] == '/')
	{
		result = strdup(path);
		if (!result)
			return (NULL);
		return (result);
	}

	result = strdup(cwd);
	if (!result)
		return (NULL);

	return (resolve_path_dots(path, cwd, result));
}
