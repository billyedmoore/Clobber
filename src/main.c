#include "header.h"

pid_t background_processes[MAX_BACKGROUND];
int number_alive_background_processes = 0;
pid_t foreground_process;
command_list command_queue;

int main(int argc, char **argv) {
  /***
   * The main function.
   */

  command_queue = create_command_queue();

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

  Command cmd;
  if (command_queue.len > 0) {
    cmd = get_next_command_from_queue();
  } else {
    char *line = malloc(100 * sizeof(char));
    char cwd[PATH_MAX];
    getcwd(cwd, sizeof(cwd));

    printf(FONT_CYAN "(%s)" FONT_GREEN " -> " FONT_COLOUR_RESET, cwd);
    fgets(line, 100, stdin);
    // parse_command disposes of the line once done with it.
    cmd = parse_command(line);
  }
  int exit_code = run_command(cmd, builtins);

  delete_command(cmd);
  return exit_code;
}
