#include "shell.h"

/**
 * main - Entry point for the shell
 *
 * This function initializes the shell, handles user input, and
 * executes commands in a loop until the user exits.
 *
 * Return: Last command's exit status
 */
int main(void)
{
    char *input;
    int last_status = 0;

    while (1)
    {
        /* Check if input is from an interactive terminal */
        if (isatty(STDIN_FILENO))
            display_prompt();

        /* Read user input */
        input = get_input();
        if (input == NULL)
            break;

        /* Process and execute the command */
        last_status = execute_command(input);
        free(input);
        if (!isatty(STDIN_FILENO))
            exit(last_status);
    }

    return (last_status);
}

/**
 * display_prompt - Displays the shell prompt
 *
 * This function prints the prompt symbol (e.g., "$ ") to indicate
 * the shell is ready to accept user input.
 */
void display_prompt(void)
{
    printf("$ ");
    fflush(stdout);
}

/**
 * get_input - Reads input from the user
 *
 * This function uses getline to read a line of input from stdin.
 * If the user inputs EOF (Ctrl+D), the function returns NULL.
 *
 * Return: Pointer to the input string, or NULL on EOF
 */
char *get_input(void)
{
    char *input = NULL;
    size_t input_size = 0;
    ssize_t input_read;

    /* Read a line of input */
    input_read = getline(&input, &input_size, stdin);
    if (input_read == EOF)
    {
        free(input);
        return (NULL);
    }

    /* Remove the newline character if present */
    if (input_read > 0 && input[input_read - 1] == '\n')
    input[input_read - 1] = '\0';

    return (input);
}

/**
 * execute_command - Processes and executes a command
 * @input: The command entered by the user
 *
 * Return: Exit status of the command
 */
int execute_command(char *input)
{
    char *args[64];       /* Array to store command and arguments */
    char *token;          /* For splitting input into tokens */
    int i = 0;
    int status = 0;
    char *trimmed_input;
    char *command_start;

    if (!input)
        return (0);

    /* Skip leading spaces and tabs */
    trimmed_input = input;
    while (*trimmed_input && (*trimmed_input == ' ' || *trimmed_input == '\t' || *trimmed_input == '\n'))
        trimmed_input++;

    /* Handle empty input or input with only spaces */
    if (*trimmed_input == '\0')
        return (0);

    /* Save start of command for later use */
    command_start = trimmed_input;

    /* Remove trailing spaces, tabs and newlines */
    i = strlen(trimmed_input) - 1;
    while (i >= 0 && (trimmed_input[i] == ' ' || trimmed_input[i] == '\t' || trimmed_input[i] == '\n'))
    {
        trimmed_input[i] = '\0';
        i--;
    }

    /* Handle empty input after trimming */
    if (*command_start == '\0')
        return (0);

    /* Handle 'exit' command */
    if (strcmp(command_start, "exit") == 0)
    {
        free(input);
        exit(0);
    }

    /* Handle 'env' command */
    if (strcmp(command_start, "env") == 0)
    {
        handle_env();
        return (0);
    }

    /* Split input into command and arguments */
    token = strtok(command_start, " \t");
    i = 0;
    while (token && i < 63)  /* Leave room for NULL terminator */
    {
        args[i] = token;
        token = strtok(NULL, " \t");
        i++;
    }
    args[i] = NULL;  /* NULL terminate argument array */

    /* Execute the command */
    if (args[0] == NULL || args[0][0] == '\0')
        return (0);

    status = execute_builtin(args[0], args);
    return (status);
}

/**
 * handle_env - Prints the environment variables
 *
 * This function iterates through the global environment variable array
 * and prints each variable to the standard output.
 */
void handle_env(void)
{
    char **env;

    for (env = environ; *env != NULL; env++)
        printf("%s\n", *env);
}
