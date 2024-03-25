#include <errno.h>
#include <linux/limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>

// Structure to store a command.
struct command {
  char **arguments;
  int count;
};

typedef struct command Command;

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

built_in_func builtins[2];

int main(int argc, char **argv) {
  /***
   * The main function.
   */

  // Define the builtins.
  builtins[0].func = &builtin_cd;
  builtins[0].name = "cd";

  builtins[1].func = &builtin_exit;
  builtins[1].name = "exit";
  if (isatty(STDIN_FILENO)) {
    while (1) {
      main_loop();
    }
  } else {
    printf("We are in a script or are we?\n");
  }
}

int main_loop() {
  /***
   * Run one command.
   */
  char *line = malloc(100 * sizeof(char));
  char cwd[PATH_MAX];
  getcwd(cwd, sizeof(cwd));

  printf("(%s) >", cwd);
  fgets(line, 100, stdin);
  Command cmd = parse_command(line);
  int exit_code = run_command(cmd);

  free(line);
  free(cmd.arguments);
  return exit_code;
}

int run_command(Command cmd) {
  /***
   * Run a command.
   *
   * Input:
   *  Command cmd: The command to be executed, made up of the arguments and
   *  number of arguments.
   * Output:
   *  int exit status: 0 -> Success, 1 -> Failure, 2 -> Usage Error.
   */

  // If the command is empty.
  if (cmd.count == 0) {
    printf("Enter a command.");
    return 2;
  }

  // Compare the first argument with each builtin function.
  // If they match execute the builtin.
  // Won't scale well if too many builtins.
  for (int i = 0; i < (sizeof(builtins) / sizeof(built_in_func)); i++) {
    if (strcmp(builtins[i].name, cmd.arguments[0]) == 0) {
      return builtins[i].func(cmd);
    }
  }

  pid_t pid = fork();
  switch (pid) {
  case -1:
    // Failed to fork
    printf("Couldn't fork.");
    return 1;
  case 0:
    // Child
    if (execvp(cmd.arguments[0], cmd.arguments) == -1) {
      switch (errno) {
      case (2):
        fprintf(stderr, "Program \"%s\" not found\n", cmd.arguments[0]);
        break;
      case (13):
        fprintf(stderr, "Permission denied\n");
        break;
      default:
        fprintf(stderr, "Couldn't execute %s due to %s\n", cmd.arguments[0],
                strerror(errno));
      }
    }
    exit(1);
  default:
    // parent
    wait(NULL);
    return 0;
  }

  // If we haven't run a command then command not found.
  printf("Command \"%s\" not found.\n", cmd.arguments[0]);

  return 1;
}

Command parse_command(char *line) {
  /***
   * Parse a command into a "list" of arguments of type char**.
   * Allocates memory up to a bufsize.
   */
  line[strcspn(line, "\n")] = 0;
  int bufsize = 5; // The amount of pointers to allocate memory for.
  int i = 0;       // The current words being handled.
  char **args = malloc(sizeof(char *) *
                       bufsize); // Allocate the memory at inital bufsize.

  // Loop through each word using strtok
  for (char *token = strtok(line, " "); token != NULL;
       token = strtok(NULL, " ")) {
    // If not enough memory has been allocated double the amount allocated.
    if (i <= bufsize) {
      bufsize = bufsize * 2;
      args = realloc(args, sizeof(char *) * bufsize);
    }
    // Assign the word (token) to the next free pointer.
    args[i] = token;
    i++;
  }
  // Reallocate memory so only the amount needed is allocated.
  args = realloc(args, (i + 1) * sizeof(char *));
  Command cmd = {args, i};
  return cmd;
}

int builtin_cd(Command cmd) {
  /***
   * Builtin "cd" command to change the current working directory.
   *
   * Input:
   *  Command cmd: The command made up of the argumentss (including "cd" as
   * the first value) and the count. Output: int exit_code : See run_command
   * for details.
   */
  if (cmd.count <= 1) {
    printf("Not enough arguements silly.");
    return 2;
  } else {
    int changed = chdir(cmd.arguments[1]);

    switch (changed) {
    case 0:
      return 0;
    case -1:
      return 1;
    default:
      return 1;
    }
  }
}

int builtin_exit(Command cmd) { exit(0); }
