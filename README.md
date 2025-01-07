# Simple Shell

A simple UNIX command line interpreter implemented in C.

## Description

This shell project is a minimalist implementation of a command-line interpreter that provides basic shell functionality including:

- Interactive command prompt
- Command execution with arguments
- PATH resolution
- Basic built-in commands (exit)
- Error handling
- Support for absolute and relative paths
- Proper handling of whitespace and empty lines

## Features

- Displays a prompt and waits for user input
- Executes commands with arguments
- Handles the PATH environment variable
- Implements built-in command: exit
- Proper error handling and memory management
- Supports both interactive and non-interactive mode
- Handles leading and trailing whitespace in commands
- Works with both absolute paths (/bin/ls) and relative paths
- Continues execution after empty lines
- Functions correctly with or without environment variables

## Compilation

```bash
gcc -Wall -Werror -Wextra -pedantic -std=gnu89 *.c -o hsh
```

## Usage

### Interactive Mode
```bash
$ ./hsh
$ ls
hsh main.c shell.h execute.c
$ pwd
/home/user/simple_shell
$ exit
```

### Non-interactive Mode
```bash
$ echo "ls" | ./hsh
hsh main.c shell.h execute.c
```

## Test Cases

The shell handles various test cases including:

1. Basic command execution:
```bash
$ ls
$ ls -l
$ pwd
```

2. Path handling:
```bash
$ /bin/ls
$ ../../directory/command
```

3. Whitespace handling:
```bash
$     ls    
$     ls -l    
```

4. Multiple commands:
```bash
$ ls
$ pwd
$ ls -l
```

5. Environment handling:
```bash
$ env -i ./hsh
$ ls    # Works without environment variables
```

6. Error handling:
```bash
$ nonexistent    # Proper error message
$ /nonexistent   # Proper error message with path
```

## Built-in Commands

1. `exit` - Exit the shell

## Author

- Frederic Bourouliou <frederic.bourouliou@gmail.com>
