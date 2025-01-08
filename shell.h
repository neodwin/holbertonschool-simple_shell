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
void execute_command(char *command, char *program_name);
char *get_path(char *command);
char *try_path(const char *dir, const char *command);
void cleanup(char *cmd_path, char **args);
char **parse_command(char *command);
void execute_in_child(char *cmd_path, char **args, char *program_name);
int handle_builtin(char **args);
void handle_exit(char **args);
char **prepare_command(char *command);
void process_commands(char *line, char *program_name);

#endif /* SHELL_H */
