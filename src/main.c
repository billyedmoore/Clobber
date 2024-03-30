#include "header.h"

pid_t background_processes[MAX_BACKGROUND];
int number_alive_background_processes = 0;
pid_t foreground_process;

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

  delete_command(cmd);
  return exit_code;
}

void delete_command(Command cmd) {
  /***
   * Deallocate all dynamically allocated memory related to cmd.
   */
  // Free all arguments.
  for (int i = 0; i < cmd.count; i++) {
    free(cmd.arguments[i]);
  }
  // Free the arguments structure.
  free(cmd.arguments);
}
