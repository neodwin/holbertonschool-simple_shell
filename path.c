#include "shell.h"

/**
 * find_path_in_environ - Searches through environ array for PATH
 *
 * This function scans environment variables to find PATH variable.
 * If PATH is not found in environment, returns a default value.
 *
 * Return: Pointer to PATH value if found, or "/bin:/usr/bin" if not found
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
            return (*path == '\0' ? NULL : path);
        }
        i++;
    }
    return (NULL);
}

/**
 * check_current_path - Check if command exists in current path
 * @command: The command to check
 *
 * This function checks if the command contains a slash, indicating
 * it's a path rather than just a command name. If it is a path,
 * duplicates the string for later use.
 *
 * Return: Duplicated command if it contains '/', NULL otherwise
 */
char *check_current_path(char *command)
{
    /* Return NULL if command is NULL */
    if (command == NULL)
        return (NULL);

    /* Check if command contains a path separator */
    if (strstr(command, "/") != NULL)
        /* If it's a path, duplicate it and return */
        return (strdup(command));

    /* Not a path, return NULL */
    return (NULL);
}

/**
 * try_path - Try to find command in given directory
 * @dir: The directory to check in
 * @command: The command name to find
 *
 * This function combines directory and command name to create a full path,
 * then checks if this path exists and is executable using stat.
 *
 * Return: Full path if command found, NULL otherwise
 */
char *try_path(char *dir, char *command)
{
    char *full_path;
    struct stat st;

    /* Allocate memory for full path (+2 for '/' and '\0') */
    full_path = malloc(strlen(dir) + strlen(command) + 2);
    if (!full_path)
        return (NULL);

    /* Combine dir and command to create full path */
    sprintf(full_path, "%s/%s", dir, command);

    /* Check if path exists and is accessible */
    if (stat(full_path, &st) == 0)
        return (full_path);

    /* Path not found, free memory and return NULL */
    free(full_path);
    return (NULL);
}

/**
 * get_command_path - Get the full path of a command
 * @command: The command name to find
 *
 * This function first checks if command contains a path, then searches
 * in PATH directories. Uses strtok to split PATH into individual directories.
 *
 * Return: Full path to command if found, NULL if not found
 */
char *get_command_path(char *command)
{
    char *path;
    char *path_copy;
    char *dir;
    char *full_path;
    struct stat st;

    if (command == NULL || *command == '\0')
        return (NULL);

    /* Check if command is an absolute or relative path */
    if (strchr(command, '/') != NULL)
    {
        if (stat(command, &st) == 0 && (st.st_mode & S_IXUSR))
            return (strdup(command));
        return (NULL);
    }

    /* Get PATH and handle empty PATH case */
    path = find_path_in_environ();
    if (!path)
        return (NULL);

    path_copy = strdup(path);
    if (!path_copy)
        return (NULL);

    dir = strtok(path_copy, ":");
    while (dir)
    {
        full_path = try_path(dir, command);
        if (full_path)
        {
            free(path_copy);
            return (full_path);
        }
        dir = strtok(NULL, ":");
    }
    free(path_copy);
    return (NULL);
}

/**
 * execute_builtin - Execute a command with its arguments
 * @command: The command to execute
 * @args: Array of command and its arguments
 *
 * Return: Exit status of the command (127 if not found, 0 on success)
 */
int execute_builtin(char *command, char **args)
{
    pid_t pid;
    char *cmd_path;
    int status = 0;

    /* Get full path of command */
    cmd_path = get_command_path(command);
    if (!cmd_path)
    {
        fprintf(stderr, "%s: 1: %s: not found\n", args[0], command);
        return (127);
    }

    /* Create new process */
    pid = fork();
    if (pid == -1)
    {
        free(cmd_path);
        perror("fork");
        return (1);
    }

    if (pid == 0)
    {
        /* Child process: execute the command */
        if (execve(cmd_path, args, environ) == -1)
        {
            fprintf(stderr, "%s: 1: %s: not found\n", args[0], command);
            free(cmd_path);
            _exit(127);
        }
    }
    else
    {
        /* Parent process: wait for child and clean up */
        waitpid(pid, &status, 0);
        free(cmd_path);
        if (WIFEXITED(status))
            return (WEXITSTATUS(status));
    }
    return (status);
}
