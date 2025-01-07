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
 * Prototypes - Functions for ls command handling
 */
int is_ls_command(const char *command);
char *handle_ls_path(const char *command);
int execute_ls(char *command, char **args);

/*
 * Prototypes - Functions for builtin commands
 */
void handle_env(void);
int process_builtin(char *input);
void execute_command_child(char *cmd_path, char **args);

/*
 * Prototypes - Functions for parsing
 */
int parse_args(char *line_copy, char **args, char *line);
int parse_command(char *line, char **args);
char *find_newline(char *str);
int process_single_command(char *line, char **args, char *program_name);

/*
 * Prototypes - Functions for command utilities
 */
char *resolve_dots(const char *path);
char *get_command_path_ext(const char *command);
int execute_command_ext(char *cmd_path, char **args);

/*
 * Prototypes - Core shell operations
 */
void display_prompt(void);
char *get_input(void);
int execute_command(char *input, char *program_name);

/*
 * Global variable - Environment access
 */
extern char **environ;

#endif /* SHELL_H */
