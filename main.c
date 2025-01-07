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
    char *line_end;
    char *current_pos;

    if (!input)
        return (0);

    /* Process each line */
    current_pos = input;
    while (*current_pos)
    {
        /* Find end of current line */
        line_end = strchr(current_pos, '\n');
        if (line_end)
            *line_end = '\0';

        /* Skip leading spaces and tabs */
        while (*current_pos == ' ' || *current_pos == '\t')
            current_pos++;

        /* Skip empty lines */
        if (*current_pos)
        {
            /* Handle 'exit' command */
            if (strcmp(current_pos, "exit") == 0)
            {
                free(input);
                exit(0);
            }

            /* Handle 'env' command */
            if (strcmp(current_pos, "env") == 0)
            {
                handle_env();
            }
            else
            {
                /* Split input into command and arguments */
                i = 0;
                token = strtok(current_pos, " \t");
                while (token && i < 63)
                {
                    args[i] = token;
                    token = strtok(NULL, " \t");
                    i++;
                }
                args[i] = NULL;

                /* Execute the command if not empty */
                if (args[0] && args[0][0] != '\0')
                    status = execute_builtin(args[0], args);
            }
        }

        /* Move to next line if there is one */
        if (line_end)
            current_pos = line_end + 1;
        else
            break;
    }

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
