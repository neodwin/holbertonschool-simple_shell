#ifndef SHELL_H
#define SHELL_H
/* Librarys */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
/* Prototypes */
extern char **environ;
char *_getenv(const char *value);
void display_prompt(void);
char *get_input(void);
void execute_command(char *input);
void handle_env(void);
#endif /* SHELL_H */