#include "header.h"

pid_t background_processes[MAX_BACKGROUND];
int number_alive_background_processes = 0;
pid_t foreground_process;
command_list command_queue;
built_in_func *builtins;
enum operating_mode operating_mode;

int main(int argc, char **argv) {
  /***
   * The main function.
   */

  command_queue = create_command_queue();
  builtins = create_builtins();

  if (!isatty(STDIN_FILENO)) {
    // IF RUNNING IN A SCRIPT
    operating_mode = SCRIPT;
    populate_command_queue();
  } else {
    // OTHERWISE RUNNING INTERACTIVELY
    operating_mode = INTERACTIVE;
  }
  while (1) {
    main_loop();
  }
}

int main_loop() {
  /***
   * Run one command.
   */

  Command cmd;
  if (command_queue.len > 0) {
    cmd = get_next_command_from_queue();
  } else if (operating_mode == INTERACTIVE) {
    char *line = prompt();
    cmd = parse_command(line);
    free(line);
  } else {
    // If not in INTERACTIVE mode and there are no more commands in the queue.
    free_before_exit();
    exit(1);
  }
  int exit_code = run_command(cmd);

  delete_command(cmd);
  return exit_code;
}

void free_before_exit() {
  /***
   * Deallocate memory before exiting.
   */
  free(builtins);
  delete_command_list(command_queue);
}
