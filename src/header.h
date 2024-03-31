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

#define NUMBER_BUILTINS 3
#define MAX_BACKGROUND 30
#define MAX_LINE_SIZE 200
#define BUFFER_SIZE 10

#define FONT_COLOUR_RESET "\x1b[0m"
#define FONT_CYAN "\e[1;96m"
#define FONT_GREEN "\e[0;92m"

extern pid_t background_processes[MAX_BACKGROUND];
extern int number_alive_background_processes;
extern pid_t foreground_process;

enum redirection_types { NONE, NORMAL, APPEND };

enum operating_mode { INTERACTIVE, SCRIPT };

// Structure to store a command.
struct command {
  char **arguments;
  int count;
  bool background;
  char *redirection_file;
  enum redirection_types redirection_type;
};

typedef struct command Command;

// Stucture to store a built in function.
struct built_in_func_t {
  int (*func)(Command cmd);
  char *name;
};

typedef struct built_in_func_t built_in_func;

struct command_list_t {
  size_t len;
  Command *commands;
  size_t allocated_len;
};

typedef struct command_list_t command_list;

int main_loop(built_in_func builtins[]);
int run_command(Command cmd, built_in_func builtins[]);
void free_before_exit();
command_list create_command_queue();
void delete_command_list(command_list cmd_lst);
Command get_next_command_from_queue();

extern command_list command_queue;

char *prompt();
Command parse_command(char *line);
void populate_command_queue();
void delete_command(Command cmd);
char **copy_string_array(char **source, int num_elements);

// Builtins
int builtin_cd(Command cmd);
int builtin_exit(Command cmd);
int builtin_help(Command cmd);

// Signal handlers
void set_handlers();

void handle_sigchld();
void handle_sigint();

#endif
