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

command_list create_command_queue() {
  /***
   * Create a command queue.
   */

  command_list queue = {
      .len = 0, .commands = malloc(sizeof(Command) * MAX_COMMAND_QUEUE_SIZE)};
  return queue;
}

Command get_next_command_from_queue() {
  // Get the next command from the queue.

  if (command_queue.len == 0) {
    // Not sure of the best way to handle this.
    printf("No command in queue.\n");
    exit(1);
  }

  Command cmd = command_queue.commands[0];

  for (int i = 0; i < (command_queue.len - 1); i++) {
    command_queue.commands[i] = command_queue.commands[i + 1];
  };
  command_queue.len--;

  return cmd;
}
