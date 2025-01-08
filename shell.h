#ifndef SHELL_H
#define SHELL_H

/******************************************************************
 * Simple Shell Header File
 *
 * This header contains includes, prototypes, and global variables
 * for the simple shell implementation. The shell executes commands,
 * handles PATH resolution, and implements basic built-in commands.
 *
 * Author: Frederic Bourouliou
 ******************************************************************/

/* Standard Library Includes */
#include <stdio.h>    /* For input/output operations */
#include <stdlib.h>   /* For memory allocation and exit */
#include <string.h>   /* For string manipulation */
#include <unistd.h>   /* For POSIX operating system API */
#include <sys/types.h>/* For system data types */
#include <sys/wait.h> /* For process control */
#include <sys/stat.h> /* For file status */

/* Global Environment Variable */
extern char **environ;

/* Function Prototypes */

/**
 * display_prompt - Display the shell prompt
 * Description: Shows the shell prompt and flushes stdout
 */
void display_prompt(void);

/**
 * read_line - Read a line from standard input
 * Description: Reads user input until newline
 * Return: Pointer to input string or NULL
 */
char *read_line(void);

/**
 * process_commands - Process input commands
 * @line: Input line to process
 * @program_name: Name of shell program
 * Description: Processes and executes commands from input
 */
void process_commands(char *line, char *program_name);

/**
 * execute_command - Execute a shell command
 * @command: Command to execute
 * @program_name: Name of shell program
 * Description: Main command execution handler
 */
void execute_command(char *command, char *program_name);

/**
 * parse_command - Parse command into arguments
 * @command: Command string to parse
 * Description: Splits command into array of arguments
 * Return: Array of argument strings
 */
char **parse_command(char *command);

/**
 * execute_in_child - Execute command in child process
 * @cmd_path: Path to command
 * @args: Command arguments
 * @program_name: Shell program name
 * Description: Executes command in forked process
 */
void execute_in_child(char *cmd_path, char **args, char *program_name);

/**
 * prepare_command - Prepare command for execution
 * @command: Command to prepare
 * Description: Prepares command by parsing and checking
 * Return: Array of prepared arguments
 */
char **prepare_command(char *command);

/**
 * get_path - Get full path of command
 * @command: Command to find
 * Description: Searches for command in PATH
 * Return: Full path if found, NULL if not
 */
char *get_path(char *command);

/**
 * try_path - Try to find command in directory
 * @dir: Directory to search
 * @command: Command to find
 * Description: Checks if command exists in directory
 * Return: Full path if found, NULL if not
 */
char *try_path(const char *dir, const char *command);

/**
 * cleanup - Clean up memory
 * @cmd_path: Command path to free
 * @args: Arguments array to free
 * Description: Frees allocated memory
 */
void cleanup(char *cmd_path, char **args);

/**
 * handle_builtin - Handle built-in commands
 * @args: Command arguments
 * Description: Processes built-in shell commands
 * Return: 1 if handled, 0 if not
 */
int handle_builtin(char **args);

/**
 * handle_exit - Handle exit command
 * @args: Exit arguments
 * Description: Processes exit command with status
 */
void handle_exit(char **args);

#endif /* SHELL_H */
