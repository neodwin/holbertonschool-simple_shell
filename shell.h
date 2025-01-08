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
extern char **environ; /* Array of environment variables */

/* Function Prototypes */

/**
 * display_prompt - Display the shell prompt
 *
 * Description: Shows the shell prompt '$ ' and flushes stdout
 * to ensure immediate display. Only used in interactive mode.
 */
void display_prompt(void);

/**
 * read_line - Read a line from standard input
 *
 * Description: Reads user input until newline or EOF is encountered.
 * Handles both interactive and non-interactive modes.
 *
 * Return: Pointer to input string or NULL on EOF/error
 */
char *read_line(void);

/**
 * process_commands - Process input commands
 * @line: Input line to process
 * @program_name: Name of shell program for error messages
 *
 * Description: Processes and executes commands from input line.
 * Handles command parsing, execution, and error reporting.
 */
void process_commands(char *line, char *program_name);

/**
 * execute_command - Execute a shell command
 * @command: Command string to execute
 * @program_name: Name of shell program for error messages
 *
 * Description: Main command execution handler. Prepares command,
 * finds full path, creates child process, executes, and waits.
 */
void execute_command(char *command, char *program_name);

/**
 * parse_command - Parse command into arguments
 * @command: Command string to parse
 *
 * Description: Splits command into array of arguments using space
 * and tab as delimiters. Allocates memory for arguments.
 *
 * Return: Array of argument strings or NULL if allocation fails
 */
char **parse_command(char *command);

/**
 * execute_in_child - Execute command in child process
 * @cmd_path: Path to command executable
 * @args: Array of command arguments
 * @program_name: Shell program name for error messages
 *
 * Description: Executes command in forked process using execve.
 * Handles execution errors and memory cleanup.
 */
void execute_in_child(char *cmd_path, char **args, char *program_name);

/**
 * prepare_command - Prepare command for execution
 * @command: Command string to prepare
 *
 * Description: Prepares command by removing leading spaces,
 * parsing into arguments, and checking for built-ins.
 *
 * Return: Array of prepared arguments or NULL if empty/built-in
 */
char **prepare_command(char *command);

/**
 * get_path - Get full path of command
 * @command: Command to find in PATH
 *
 * Description: Searches for command in directories listed in PATH.
 * Handles both absolute paths and commands without path.
 *
 * Return: Full path if found, NULL if not found
 */
char *get_path(char *command);

/**
 * try_path - Try to find command in directory
 * @dir: Directory to search in
 * @command: Command to find
 *
 * Description: Checks if command exists and is executable in
 * specified directory. Constructs full path if found.
 *
 * Return: Full path if found and executable, NULL otherwise
 */
char *try_path(const char *dir, const char *command);

/**
 * cleanup - Clean up memory
 * @cmd_path: Command path to free
 * @args: Arguments array to free
 *
 * Description: Frees memory allocated for command path and
 * argument array. Handles NULL pointers safely.
 */
void cleanup(char *cmd_path, char **args);

/**
 * handle_builtin - Handle built-in commands
 * @args: Command arguments to process
 *
 * Description: Processes built-in shell commands like exit and env.
 * Executes appropriate built-in function if command matches.
 *
 * Return: 1 if command was built-in, 0 otherwise
 */
int handle_builtin(char **args);

/**
 * handle_exit - Handle exit command
 * @args: Exit command arguments
 *
 * Description: Processes exit command with optional status.
 * Cleans up memory and exits shell with specified status.
 */
void handle_exit(char **args);

#endif /* SHELL_H */
