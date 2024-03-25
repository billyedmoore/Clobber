#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

// Structure to store a command.
struct command {
  char **arguments;
  int count;
};

typedef struct command Command;

int run_command(Command cmd);
Command parse_command(char *line);

int main(int argc, char **argv) {
  /***
   * The main function.
   */
  if (isatty(STDIN_FILENO)) {
    char *line = malloc(100 * sizeof(char));
    printf("Hey >");
    fgets(line, 100, stdin);
    Command cmd = parse_command(line);
    run_command(cmd);
    printf("%i\n", cmd.count);

    free(cmd.arguments);
  } else {
    printf("We are in a script or are we?\n");
  }
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
