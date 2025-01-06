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

    /* Loop through environment variables until PATH is found */
    while (environ[i])
    {
        /* Check if current env variable starts with "PATH=" */
        if (strncmp(environ[i], "PATH=", 5) == 0)
        {
            /* Skip "PATH=" prefix to get the value */
            path = environ[i] + 5;
            break;
        }
        i++;
    }
    /* Return found PATH or default value if not found */
    return (path ? path : "/bin:/usr/bin");
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

    /* First check if command is a path */
    full_path = check_current_path(command);
    if (full_path)
        return (full_path);

    /* Get PATH and create a copy for modification */
    path = find_path_in_environ();
    path_copy = malloc(strlen(path) + 1);
    if (!path_copy)
        return (NULL);
    strcpy(path_copy, path);

    /* Try each directory in PATH */
    dir = strtok(path_copy, ":");
    while (dir)
    {
        /* Try to find command in current directory */
        full_path = try_path(dir, command);
        if (full_path)
        {
            free(path_copy);
            return (full_path);
        }
        /* Move to next directory in PATH */
        dir = strtok(NULL, ":");
    }
    /* Command not found in any directory */
    free(path_copy);
    return (NULL);
}
/**
 * execute_builtin - Execute a command with its arguments
 * @command: The command to execute
 * @args: Array of command and its arguments
 *
 * This function creates a child process to run the command. First gets
 * the full path of the command, then forks and executes it. Parent
 * waits for child to complete.
 */
void execute_builtin(char *command, char **args)
{
    pid_t pid;
    char *cmd_path;

    /* Get full path of command */
    cmd_path = get_command_path(command);
    if (!cmd_path)
    {
        fprintf(stderr, "./hsh: 1: %s: not found\n", command);
        exit(127);
    }

    /* Create new process */
    pid = fork();
    if (pid == -1)
    {
        free(cmd_path);
        perror("fork");
        exit(1);
    }

    if (pid == 0)
    {
        /* Child process: execute the command */
        if (execve(cmd_path, args, environ) == -1)
        {
            perror("execve");
            free(cmd_path);
            exit(127);
        }
    }
    else
    {
        /* Parent process: wait for child and clean up */
        int status;
        waitpid(pid, &status, 0);
        free(cmd_path);
        if (WIFEXITED(status))
            exit(WEXITSTATUS(status));
    }
}
