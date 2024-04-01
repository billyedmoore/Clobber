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

command_list create_command_list() {
  /***
   * Create a command queue.
   */

  command_list queue = {.len = 0,
                        .commands = malloc(sizeof(Command) * BUFFER_SIZE),
                        .allocated_len = BUFFER_SIZE};
  return queue;
}

command_list append_command_list(command_list cmd_lst, Command cmd) {
  /***
   * Append command to command list.
   */
  // If there is not enough space to store another command allocate more
  // memory for it.
  if (cmd_lst.len >= cmd_lst.allocated_len) {
    cmd_lst.allocated_len = cmd_lst.allocated_len * 2;
    cmd_lst.commands =
        realloc(cmd_lst.commands, sizeof(Command) * cmd_lst.allocated_len);
  }
  // Add command to the queue.
  cmd_lst.commands[cmd_lst.len] = cmd;
  cmd_lst.len += 1;

  return cmd_lst;
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
