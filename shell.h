#ifndef SHELL_H
#define SHELL_H

/*
 * Libraries - Standard C libraries required for shell operations
 * stdio.h: Input/output operations
 * stdlib.h: Memory management, program utilities
 * string.h: String manipulation functions
 * unistd.h: POSIX operating system API
 * sys/types.h, sys/wait.h: Process control
 * sys/stat.h: File status
 * errno.h: System error numbers
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
 * These functions handle PATH environment variable operations
 * and command path resolution
 */
char *find_path_in_environ(void);
char *check_current_path(char *command);
char *try_path(char *dir, char *command);
char *get_command_path(char *command);
int execute_builtin(char *command, char **args);

/*
 * Main Functions - Core shell operations
 * These functions handle the main shell operations including
 * prompt display, input processing, and command execution
 */
void display_prompt(void);
char *get_input(void);
void execute_command(char *input);
void handle_env(void);

/*
 * Global variable - Environment access
 * environ: Array of environment variables
 */
extern char **environ;

#endif /* SHELL_H */
