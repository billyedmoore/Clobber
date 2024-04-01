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

  set_handlers();
  command_queue = create_command_list();
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

  // If interactive mode and nothing in the queue:
  //  show a prompt
  //  parse it to get a queue of commands
  //  append commands to the command_queue
  //
  if (command_queue.len == 0 && operating_mode == INTERACTIVE) {
    char *line = prompt();
    command_list cmds = parse_line(line);
    for (int i = 0; i < cmds.len; i++) {
      command_queue = append_command_list(command_queue, cmds.commands[i]);
    }
    delete_command_list(cmds);
    free(line);
  }

  // If theres a command in the queue run it.
  if (command_queue.len > 0) {
    Command cmd = get_next_command_from_queue();
    int exit_code = run_command(cmd);
    delete_command(cmd);
    return exit_code;
  } else {
    // If not in INTERACTIVE mode and there are no more commands in the queue.
    free_before_exit();
    exit(1);
  }
}

void free_before_exit() {
  /***
   * Deallocate memory before exiting.
   */
  free(builtins);
  delete_command_list(command_queue);
}
