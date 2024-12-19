#include "shell.h"

/**
 * main - Entry point for the shell
 *
 * Return: 0 on success
 */
int main(void)
{
    char *input;

    while (1)
    {
        if (isatty(STDIN_FILENO))
            display_prompt();

        input = get_input();
        if (input == NULL)
            break;

        execute_command(input);
        free(input);
    }

    return (0);
}

/**
 * display_prompt - Displays the shell prompt
 */
void display_prompt(void)
{
    printf("$ ");
    fflush(stdout);
}

/**
 * get_input - Reads input from the user
 *
 * Return: Pointer to the input string, or NULL on EOF
 */
char *get_input(void)
{
    char *input = NULL;
    size_t input_size = 0;
    ssize_t input_read;

    input_read = getline(&input, &input_size, stdin);
    if (input_read == EOF)
    {
        free(input);
        return (NULL);
    }

    if (input_read > 0 && input[input_read - 1] == '\n')
        input[input_read - 1] = '\0';

    return (input);
}

/**
 * execute_command - Processes and executes a command
 * @input: The command entered by the user
 */
void execute_command(char *input)
{
    int i = 0;

    while (input[i] == ' ')
        i++;

    if (strcmp(&input[i], "exit") == 0)
    {
        free(input);
        exit(0);
    }

    if (strcmp(&input[i], "env") == 0)
    {
        handle_env();
    }
    else if (input[i] != '\0')
    {
        printf("Commande inconnue : %s\n", &input[i]);
    }
}

/**
 * handle_env - Prints the environment variables
 */
void handle_env(void)
{
    char **env;

    for (env = environ; *env != NULL; env++)
        printf("%s\n", *env);
}