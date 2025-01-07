#include "shell.h"
#include <limits.h>

/**
 * normalize_path - Normalize a path by resolving ./ and ../
 * @path: Path to normalize
 *
 * Return: Normalized path (must be freed) or NULL on error
 */
char *normalize_path(const char *path)
{
	char resolved[PATH_MAX];
	char *norm_path = NULL;
	char *pwd = getcwd(NULL, 0);
	
	if (!pwd)
		return (NULL);

	if (realpath(path, resolved))
	{
		norm_path = strdup(resolved);
	}
	else
	{
		/* Try to resolve relative to current directory */
		char full_path[PATH_MAX];
		snprintf(full_path, sizeof(full_path), "%s/%s", pwd, path);
		if (realpath(full_path, resolved))
			norm_path = strdup(resolved);
	}

	free(pwd);
	return (norm_path);
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
	char *norm_path;
	int result = 0;

	/* Skip leading spaces */
	while (*cmd == ' ' || *cmd == '\t')
		cmd++;

	/* Check for simple "ls" command */
	if (strcmp(cmd, "ls") == 0)
		return (1);

	/* Try to normalize the path */
	norm_path = normalize_path(cmd);
	if (norm_path)
	{
		/* Check if normalized path ends with "/ls" */
		size_t len = strlen(norm_path);
		if (len >= 3 && strcmp(norm_path + len - 3, "/ls") == 0)
			result = 1;
		free(norm_path);
	}

	return (result);
}

/**
 * handle_ls_path - Handle ls command with PATH variations
 * @command: Command to handle
 *
 * Return: Full path to ls command if found, NULL otherwise
 */
char *handle_ls_path(const char *command)
{
	char *ls_path;
	struct stat st;
	const char *cmd = command;
	char *norm_path;

	/* Skip leading spaces */
	while (*cmd == ' ' || *cmd == '\t')
		cmd++;

	/* If it's a path (contains slashes), try to normalize it */
	if (strchr(cmd, '/'))
	{
		norm_path = normalize_path(cmd);
		if (norm_path)
		{
			if (stat(norm_path, &st) == 0 && (st.st_mode & S_IXUSR))
				return (norm_path);
			free(norm_path);
		}
		return (NULL);
	}

	/* Always try /bin/ls first */
	if (stat("/bin/ls", &st) == 0 && (st.st_mode & S_IXUSR))
		return (strdup("/bin/ls"));

	/* If /bin/ls doesn't work, try PATH */
	ls_path = try_path("/bin", "ls");
	if (ls_path)
		return (ls_path);

	return (NULL);
}

/**
 * execute_ls - Execute ls command with arguments
 * @command: Original command
 * @args: Command arguments
 *
 * Return: Exit status of the command
 */
int execute_ls(char *command, char **args)
{
	pid_t pid;
	char *ls_path;
	int status = 0;

	ls_path = handle_ls_path(command);
	if (!ls_path)
	{
		fprintf(stderr, "./hsh: 1: %s: not found\n", command);
		return (127);
	}

	pid = fork();
	if (pid == -1)
	{
		free(ls_path);
		perror("fork");
		return (1);
	}

	if (pid == 0)
	{
		execve(ls_path, args, environ);
		fprintf(stderr, "%s: 1: %s: not found\n", args[0], args[0]);
		free(ls_path);
		_exit(127);
	}
	else
	{
		waitpid(pid, &status, 0);
		free(ls_path);
		if (WIFEXITED(status))
			return (WEXITSTATUS(status));
	}
	return (status);
} 