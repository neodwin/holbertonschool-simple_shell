#include "shell.h"
#include <stdlib.h>
#include <string.h>

/**
 * parse_command - Parse command into arguments
 * @command: Command to parse
 * Return: Array of arguments
 */
char **parse_command(char *command)
{
	char **args = NULL;
	char *token;
	int i = 0;
	char *command_copy = strdup(command);

	if (!command_copy)
		return (NULL);

	args = malloc(sizeof(char *) * MAX_ARGS);
	if (!args)
	{
		free(command_copy);
		return (NULL);
	}

	token = strtok(command_copy, " \t\n");
	while (token && i < MAX_ARGS - 1)
	{
		args[i] = strdup(token);
		if (!args[i])
		{
			cleanup(NULL, args);
			free(command_copy);
			return (NULL);
		}
		token = strtok(NULL, " \t\n");
		i++;
	}
	args[i] = NULL;
	free(command_copy);

	return (args);
}

/**
 * cleanup - Free memory allocated for command execution
 * @cmd_path: Command path to free
 * @args: Arguments array to free
 */
void cleanup(char *cmd_path, char **args)
{
	int i;

	if (cmd_path)
		free(cmd_path);

	if (args)
	{
		for (i = 0; args[i]; i++)
			free(args[i]);
		free(args);
	}
}

/**
 * prepare_command - Prepare command for execution
 * @command: Command to prepare
 * Return: Trimmed command
 */
char *prepare_command(char *command)
{
	char *trimmed_command = command;

	while (*trimmed_command == ' ' || *trimmed_command == '\t')
		trimmed_command++;

	return (trimmed_command);
}
