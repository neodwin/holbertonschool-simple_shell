#ifndef SHELL_H
#define SHELL_H

/* Librarys */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <errno.h>

/* Prototypes */
extern char **environ;
char *_getenv(const char *value);
void display_prompt(void);
char *get_input(void);
void execute_command(char *input);
void handle_env(void);
char *find_path_in_environ(void);
char *check_current_path(char *command);
char *try_path(char *dir, char *command);
char *get_command_path(char *command);

/* Global variable */
extern char **environ;

#endif /* SHELL_H */
