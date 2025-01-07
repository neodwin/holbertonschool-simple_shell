#include "shell.h"

/**
 * is_ls_command - Check if command is ls
 * @command: Command to check
 *
 * Return: 1 if ls command, 0 otherwise
 */
int is_ls_command(const char *command)
{
	const char *cmd = command;

	/* Skip leading spaces */
	while (*cmd == ' ' || *cmd == '\t')
		cmd++;

	/* Check for "ls" or path ending with "/ls" */
	if (strcmp(cmd, "ls") == 0)
		return (1);

	if (strstr(cmd, "/ls") != NULL)
	{
		const char *slash_ls = strstr(cmd, "/ls");
		if (*(slash_ls + 3) == '\0' || *(slash_ls + 3) == ' ' ||
			*(slash_ls + 3) == '\t')
			return (1);
	}

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
	char *ls_path;
	struct stat st;
	const char *cmd = command;

	/* Skip leading spaces */
	while (*cmd == ' ' || *cmd == '\t')
		cmd++;

	/* If it's a full path to ls */
	if (strchr(cmd, '/'))
	{
		if (stat(cmd, &st) == 0 && (st.st_mode & S_IXUSR))
			return (strdup(cmd));
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
		fprintf(stderr, "%s: 1: %s: not found\n", args[0], command);
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