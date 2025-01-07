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

#define MAX_ARGS 64
#define MAX_PATH_LENGTH 1024

/* Global environment variable */
extern char **environ;

void display_prompt(void);
ssize_t read_input(char *buffer, size_t size);
void process_line(char *line, char *program_name);
int handle_builtin(char **args);
void handle_exit(char **args);
char *get_path(char *command);
char *find_path_in_env(void);
char *try_path(const char *dir, const char *command);
void cleanup(char *cmd_path, char **args);
void execute_command_helper(char *cmd_path, char **args, char *program_name);
char **parse_command(char *command);
void execute_in_child(char *cmd_path, char **args, char *program_name);
void execute_command(char *command, char *program_name);
char *prepare_command(char *command);

#endif /* SHELL_H */
