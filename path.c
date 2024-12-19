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
