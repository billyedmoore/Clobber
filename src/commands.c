#include "header.h"

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

void delete_command_list(command_list cmd_lst) {
  /***
   * Deallocate memory allocated for a command_list.
   */

  free(cmd_lst.commands);
}

Command get_next_command_from_queue() {
  /***
   * Get the next command from the command_queue remove it from the queue and
   * return it.
   */

  if (command_queue.len == 0) {
    // Not sure of the best way to handle this.
    printf("No command in queue.\n");
    exit(1);
  }

  Command cmd = command_queue.commands[0];

  // Move the commands forward.
  for (int i = 0; i < (command_queue.len - 1); i++) {
    command_queue.commands[i] = command_queue.commands[i + 1];
  };
  command_queue.len--;

  return cmd;
}
