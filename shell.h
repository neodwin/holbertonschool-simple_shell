#ifndef SHELL_H
#define SHELL_H

/*
 * Libraries - Standard C libraries required for shell operations
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <errno.h>

/*
 * Prototypes - Functions for path handling and command execution
 */
char *find_path_in_environ(void);
char *check_current_path(char *command);
char *try_path(char *dir, char *command);
char *get_command_path(char *command);
int execute_builtin(char *command, char **args);

/*
 * Prototypes - Functions for builtin commands
 */
void handle_env(void);
int process_builtin(char *input);
void execute_command_child(char *cmd_path, char **args);

/*
 * Prototypes - Core shell operations
 */
void display_prompt(void);
char *get_input(void);
int execute_command(char *input);

/*
 * Global variable - Environment access
 */
extern char **environ;

#endif /* SHELL_H */
