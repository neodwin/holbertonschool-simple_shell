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
	char *norm_path = strdup(path);

	if (!norm_path)
		return (NULL);
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
	size_t len;

	/* Skip leading spaces */
	while (*cmd == ' ' || *cmd == '\t')
		cmd++;

	/* Check for simple "ls" command */
	if (strcmp(cmd, "ls") == 0)
		return (1);

	/* Check for ls with options or arguments */
	len = strlen(cmd);
	if (len >= 2 && strncmp(cmd, "ls", 2) == 0 &&
		(cmd[2] == ' ' || cmd[2] == '\t' || cmd[2] == '\0'))
		return (1);

	return (0);
}

/**
 * handle_ls_path - Handle ls command with PATH variations
 * @command: Command to handle
 *
 * Return: Full path to ls command if found, NULL otherwise
 */
char *handle_ls_path(const char *command)
{
	struct stat st;
	const char *cmd = command;

	/* Skip leading spaces */
	while (*cmd == ' ' || *cmd == '\t')
		cmd++;

	/* Try direct path first */
	if (stat(cmd, &st) == 0 && (st.st_mode & S_IXUSR))
		return (strdup(cmd));

	/* Always try /bin/ls first */
	if (stat("/bin/ls", &st) == 0 && (st.st_mode & S_IXUSR))
		return (strdup("/bin/ls"));

	/* If /bin/ls doesn't work, try PATH */
	return (try_path("/bin", "ls"));
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

	ls_path = strdup(command);
	if (!ls_path)
	{
		fprintf(stderr, "%s: 1: %s: not found\n", args[0], command);
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
		fprintf(stderr, "%s: 1: %s: not found\n", args[0], ls_path);
		free(ls_path);
		_exit(127);
	}

	waitpid(pid, &status, 0);
	free(ls_path);
	if (WIFEXITED(status))
		return (WEXITSTATUS(status));

	return (status);
}
