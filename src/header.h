#ifndef HEADER_H
#define HEADER_H

#include <errno.h>
#include <linux/limits.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>

#define FONT_COLOUR_RESET "\x1b[0m"
#define FONT_CYAN "\e[1;96m"
#define FONT_GREEN "\e[0;92m"

// Structure to store a command.
struct command {
  char **arguments;
  int count;
  bool background;
};

typedef struct command Command;

// Stucture to store a built in function.
struct built_in_func_t {
  int (*func)(Command cmd);
  char *name;
};

typedef struct built_in_func_t built_in_func;

int main_loop();
int run_command(Command cmd);
Command parse_command(char *line);

// Builtins
int builtin_cd(Command cmd);
int builtin_exit(Command cmd);
int builtin_help(Command cmd);

#endif
