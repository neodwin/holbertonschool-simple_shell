#ifndef SHELL_H
#define SHELL_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>

#define MAX_ARGS 32

extern char **environ;

/* execute.c */
int execute_command(char *command, char *program_name);
int execute_in_child(char *command, char **args, char *program_name);
void cleanup(char **args);
char **parse_command(char *command);
char *prepare_command(char *command);

/* path.c */
char *get_path(const char *command, char **env);
char *find_path_in_env(char **env);
char *try_path(const char *dir, const char *command);
char *normalize_path(const char *path);

/* builtins.c */
int handle_builtin(char *command);
void handle_exit(void);

/* main.c */
ssize_t read_input(char *buffer, size_t size);
void display_prompt(void);

#endif /* SHELL_H */
