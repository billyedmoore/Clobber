#include "header.h"

int main(int argc, char **argv) {
  /***
   * The main function.
   */
  built_in_func builtins[NUMBER_BUILTINS];

  // Define the builtins.
  builtins[0].func = &builtin_cd;
  builtins[0].name = "cd";

  builtins[1].func = &builtin_exit;
  builtins[1].name = "exit";

  builtins[2].func = &builtin_help;
  builtins[2].name = "help";

  if (isatty(STDIN_FILENO)) {
    while (1) {
      main_loop(builtins);
    }
  } else {
    printf("We are in a script or are we?\n");
  }
}

int main_loop(built_in_func builtins[]) {
  /***
   * Run one command.
   */
  char *line = malloc(100 * sizeof(char));
  char cwd[PATH_MAX];
  getcwd(cwd, sizeof(cwd));

  printf(FONT_CYAN "(%s)" FONT_GREEN " -> " FONT_COLOUR_RESET, cwd);
  fgets(line, 100, stdin);
  Command cmd = parse_command(line);
  int exit_code = run_command(cmd, builtins);

  free(line);
  free(cmd.arguments);
  return exit_code;
}

int run_command(Command cmd, built_in_func builtins[]) {
  /***
   * Run a command.
   *
   * Input:
   *  Command cmd: The command to be executed, made up of the arguments and
   *  number of arguments.
   *  built_in_func[] builtins: The builtin functions that can be used.
   * Output:
   *  int exit status: 0 -> Success, 1 -> Failure, 2 -> Usage Error.
   */

  // If the command is empty.
  if (cmd.count == 0) {
    printf("Enter a command.\n");
    return 2;
  }

  // Compare the first argument with each builtin function.
  // If they match execute the builtin.
  // Won't scale well if too many builtins.
  for (int i = 0; i < NUMBER_BUILTINS; i++) {
    if (strcmp(builtins[i].name, cmd.arguments[0]) == 0) {
      return builtins[i].func(cmd);
    }
  }

  pid_t pid = fork();
  switch (pid) {
  case -1:
    // Failed to fork
    printf("Couldn't fork.\n");
    return 1;
  case 0: {
    // Child
    int exit_code = execvp(cmd.arguments[0], cmd.arguments);
    if (exit_code == -1) {
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
        break;
      }
    }
    exit(1);
    break;
  }
  default:
    // parent
    if (!cmd.background) {
      wait(NULL);
    }
    return 0;
  }

  // If we haven't run a command then command not found.
  printf("Command \"%s\" not found.\n", cmd.arguments[0]);

  return 1;
}
