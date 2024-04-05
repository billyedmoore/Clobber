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
// Used anytime memory is dynamically allocated.
#define BUFFER_SIZE 10

#define FONT_COLOUR_RESET "\x1b[0m"
#define FONT_CYAN "\e[1;96m"
#define FONT_GREEN "\e[0;92m"

// DEFINING A COMMAND
//
//
enum redirection_types { NONE, NORMAL, APPEND };

// Structure to store a command.
struct command {
  char **arguments;
  int count;
  bool background;
  char *redirection_file;
  enum redirection_types redirection_type;
  bool piped;
};
typedef struct command Command;
int run_command(Command cmd);

// THE STATE OF THE PROGRAM
//
extern pid_t background_processes[MAX_BACKGROUND];
extern int number_alive_background_processes;
extern pid_t foreground_process;
enum operating_mode { INTERACTIVE, SCRIPT };

// Stucture to store a built in function.
typedef struct built_in_func_t built_in_func;
struct built_in_func_t {
  int (*func)(Command cmd);
  char *name;
};

// Structure to store a list of commands
typedef struct command_list_t command_list;
struct command_list_t {
  size_t len;
  Command *commands;
  size_t allocated_len;
};
extern command_list command_queue;
command_list create_command_list();
command_list append_command_list(command_list cmd_lst, Command cmd);
void delete_command_list(command_list cmd_lst);
void populate_command_queue();
Command get_next_command_from_queue();

// Structure to store a series of commands that are to be piped to each other.
struct command_batch_t {
  command_list cmd_lst;
  int **pipes;
};
typedef struct command_batch_t command_batch;
command_batch create_command_batch(command_list cmd_lst);
void delete_command_batch(command_batch batch);

struct execution_queue_t {
  command_batch *batches;
  // The number of batches
  size_t len;
  // The amount of memory allocated
  size_t allocated_len;
};
typedef struct execution_queue_t execution_queue;

extern built_in_func *builtins;
built_in_func *create_builtins();

int main_loop();
char *prompt();
void free_before_exit();

struct split_line_t {
  char **splits;
  size_t count;
  size_t allocated_size;
};
typedef struct split_line_t split_line;

split_line split_on_symbol(char *line, char sym);
split_line create_split_line();
split_line append_split(split_line sl, char *token);
void delete_split_line(split_line sl);

command_list parse_line(char *line);
Command parse_one_command(char *line);
void delete_command(Command cmd);
Command create_command(char **args, int arg_count, bool background,
                       char *redirect_location,
                       enum redirection_types redirect_type, bool piped);
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
