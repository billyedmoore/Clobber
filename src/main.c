#include "header.h"

bool pending_exit = false;
pid_t background_processes[MAX_BACKGROUND];
int number_alive_background_processes = 0;
pid_t foreground_process;
execution_queue queue;
built_in_func *builtins;
enum operating_mode operating_mode;

int main(int argc, char **argv) {
  /***
   * The main function.
   */

  set_handlers();
  queue = create_execution_queue();
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
  if (queue.len == 0 && operating_mode == INTERACTIVE && !pending_exit) {
    char *line = prompt();
    command_list cmds = parse_line(line);
    command_batch batch = create_command_batch(cmds);
    queue = append_to_execution_queue(queue, batch);
    free(line);
  }

  // If theres a command in the queue run it.
  if (queue.len > 0 && !pending_exit) {
    command_batch batch = get_next_batch_from_queue();
    int exit_code = run_command_batch(batch);
    delete_command_batch(batch);
    return exit_code;
  } else {
    // If not in INTERACTIVE mode and there are no more commands in the queue.
    free_before_exit();
    exit(0);
  }
}

void free_before_exit() {
  /***
   * Deallocate memory before exiting.
   */
  free(builtins);
  delete_execution_queue(queue);
}
