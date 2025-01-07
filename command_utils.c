#include "shell.h"

/**
 * resolve_dots - Resolve dots in path
 * @path: Path to resolve
 *
 * Return: Resolved path (must be freed) or NULL
 */
char *resolve_dots(const char *path)
{
	char *resolved = strdup(path);
	char *result;
	char *last_slash;
	char cwd[PATH_MAX];
	char *path_copy;
	char *final_path;
	char *token;

	if (!resolved || !getcwd(cwd, sizeof(cwd)))
	{
		free(resolved);
		return (NULL);
	}

	/* Start with current directory */
	result = strdup(cwd);
	if (!result)
	{
		free(resolved);
		return (NULL);
	}

	/* Make a copy for tokenization */
	path_copy = strdup(path);
	if (!path_copy)
	{
		free(resolved);
		free(result);
		return (NULL);
	}

	/* Process each part of the path */
	token = strtok(path_copy, "/");
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
		token = strtok(NULL, "/");
	}

	final_path = strdup(result);
	free(resolved);
	free(result);
	free(path_copy);
	return (final_path);
}

/**
 * get_command_path_ext - Get command path with extended checks
 * @command: Command to check
 *
 * Return: Command path (must be freed) or NULL
 */
char *get_command_path_ext(const char *command)
{
	char *cmd_path;
	struct stat st;
	char cwd[PATH_MAX];
	char *full_path;

	fprintf(stderr, "Debug: Checking command: %s\n", command);

	if (!command || !*command)
		return (NULL);

	if (command[0] == '/')
	{
		fprintf(stderr, "Debug: Absolute path detected\n");
		if (stat(command, &st) == 0 && (st.st_mode & S_IXUSR))
			return (strdup(command));
		return (NULL);
	}

	if (command[0] == '.')
	{
		fprintf(stderr, "Debug: Relative path detected\n");
		if (!getcwd(cwd, sizeof(cwd)))
			return (NULL);

		fprintf(stderr, "Debug: Current working directory: %s\n", cwd);

		full_path = malloc(PATH_MAX);
		if (!full_path)
			return (NULL);

		snprintf(full_path, PATH_MAX, "%s/%s", cwd, command);
		fprintf(stderr, "Debug: Full path before resolution: %s\n", full_path);

		cmd_path = resolve_dots(full_path);
		fprintf(stderr, "Debug: Resolved path: %s\n", cmd_path ? cmd_path : "NULL");

		free(full_path);

		if (cmd_path && stat(cmd_path, &st) == 0 && (st.st_mode & S_IXUSR))
		{
			fprintf(stderr, "Debug: Found executable at: %s\n", cmd_path);
			return (cmd_path);
		}

		fprintf(stderr, "Debug: Path not executable or not found\n");
		free(cmd_path);
		return (NULL);
	}

	fprintf(stderr, "Debug: Searching in PATH\n");
	cmd_path = get_command_path((char *)command);
	if (!cmd_path)
		return (NULL);

	if (stat(cmd_path, &st) != 0 || !(st.st_mode & S_IXUSR))
	{
		free(cmd_path);
		return (NULL);
	}

	return (cmd_path);
}

/**
 * execute_command_ext - Execute command with error handling
 * @cmd_path: Command path
 * @args: Command arguments
 *
 * Return: Exit status
 */
int execute_command_ext(char *cmd_path, char **args)
{
	pid_t pid;
	int status = 0;
	char **exec_args;
	int i, arg_count;

	/* Count arguments */
	for (arg_count = 1; args[arg_count]; arg_count++)
		;

	/* Create new argument array */
	exec_args = malloc(sizeof(char *) * (arg_count + 1));
	if (!exec_args)
	{
		free(cmd_path);
		return (1);
	}

	/* Set up arguments */
	exec_args[0] = cmd_path;
	for (i = 2; i < arg_count; i++)
		exec_args[i - 1] = args[i];
	exec_args[arg_count - 1] = NULL;

	pid = fork();
	if (pid == -1)
	{
		free(cmd_path);
		free(exec_args);
		perror("fork");
		return (1);
	}

	if (pid == 0)
	{
		execve(cmd_path, exec_args, environ);
		perror("execve");
		free(cmd_path);
		free(exec_args);
		_exit(127);
	}

	waitpid(pid, &status, 0);
	free(cmd_path);
	free(exec_args);
	if (WIFEXITED(status))
		return (WEXITSTATUS(status));

	return (status);
}
