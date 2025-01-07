#include "shell.h"

/**
 * is_ls_command - Check if command is ls
 * @command: Command to check
 *
 * Return: 1 if ls command, 0 otherwise
 */
int is_ls_command(const char *command)
{
	return (strcmp(command, "ls") == 0 || strstr(command, "/ls") != NULL);
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

	/* If it's a full path to ls */
	if (strchr(command, '/'))
	{
		if (stat(command, &st) == 0 && (st.st_mode & S_IXUSR))
			return (strdup(command));
		return (NULL);
	}

	/* Try /bin/ls first */
	ls_path = try_path("/bin", "ls");
	if (ls_path)
		return (ls_path);

	/* If PATH is empty or not found, still try /bin/ls */
	if (stat("/bin/ls", &st) == 0 && (st.st_mode & S_IXUSR))
		return (strdup("/bin/ls"));

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