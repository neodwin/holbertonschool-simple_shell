#ifndef SHELL_H
#define SHELL_H

/* Standard Libraries */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>

/* Global environment variable */
extern char **environ;

/* Function prototypes */
void display_prompt(void);
char *read_line(void);
char **split_line(char *line);
int execute_command(char **args, char *program_name);
void free_args(char **args);
int handle_builtin(char **args);
char *get_path(char *command);
void execute_in_child(char *cmd_path, char **args, char *program_name);
void handle_exit(char **args);
char *try_path(const char *dir, const char *command);

#endif /* SHELL_H */
