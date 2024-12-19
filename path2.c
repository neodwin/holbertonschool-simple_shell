#include "shell.h"

/**
 * get_path - check the path
 * @command: a string of command
 * Return: the path of command
 */

char *get_path(char *command)
{
	char *path;
	int cmd_len;
	char *token, *path_copy, *file_path;
	struct stat st;

	if (command == NULL)
		exit(EXIT_FAILURE);
	cmd_len = strlen(command);
	if (strstr(command, "/") == NULL)
	{
		path = _getenv("PATH");
		path_copy = malloc(strlen(path) + 1);
		if (path_copy == NULL)
			perror("malloc"), exit(EXIT_FAILURE);
		strcpy(path_copy, path);
		token = strtok(path_copy, ":");
		while (token)
		{
			file_path = malloc(cmd_len + strlen(token) + 2);
			if (file_path == NULL)
			{
				free(path_copy), free(file_path);
				perror("malloc"), exit(EXIT_FAILURE);
			}
			sprintf(file_path, "%s/%s", token, command);
			if (stat(file_path, &st) == 0)
			{
				free(path_copy);
				return (file_path);
			}
			free(file_path);
			token = strtok(NULL, ":");
		}
		free(path_copy);
		return (NULL);
	}
	return (command);
}