# Simple Shell

A simple UNIX command line interpreter implemented in C.

## Description

This shell project is a minimalist implementation of a command-line interpreter that provides basic shell functionality including:

- Interactive command prompt
- Command execution with arguments
- PATH resolution
- Basic built-in commands (exit, env)
- Error handling

## Features

- Displays a prompt and waits for user input
- Executes commands with arguments
- Handles the PATH environment variable
- Implements built-in commands: exit and env
- Proper error handling and memory management
- Supports both interactive and non-interactive mode

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

## Built-in Commands

1. `exit` - Exit the shell
2. `env` - Print the current environment

## Author

- Frederic Bourouliou <frederic.bourouliou@gmail.com>
