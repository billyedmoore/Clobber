#include "header.h"

pid_t background_processes[MAX_BACKGROUND];
int number_alive_background_processes = 0;
pid_t foreground_process;
command_list command_queue;
enum operating_mode operating_mode;

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

  if (!isatty(STDIN_FILENO)) {
    // IF RUNNING IN A SCRIPT
    operating_mode = SCRIPT;
    char *line = malloc(100 * sizeof(char));
    while (fgets(line, 100, stdin) != NULL) {
      Command cmd = parse_command(line);

      if (command_queue.len >= command_queue.allocated_len) {
        command_queue.allocated_len = command_queue.allocated_len * 2;
        // Allocate more memory to store more commands
        command_queue.commands =
            realloc(command_queue.commands,
                    sizeof(Command) * command_queue.allocated_len);
      }
      command_queue.commands[command_queue.len] = cmd;
      command_queue.len += 1;
    }
    free(line);
  } else {
    // OTHERWISE RUNNING INTERACTIVELY
    operating_mode = INTERACTIVE;
  }
  while (1) {
    main_loop(builtins);
  }
}

int main_loop(built_in_func builtins[]) {
  /***
   * Run one command.
   */

  Command cmd;
  if (command_queue.len > 0) {
    cmd = get_next_command_from_queue();
  } else if (operating_mode == INTERACTIVE) {
    char *line = malloc(100 * sizeof(char));
    char cwd[PATH_MAX];
    getcwd(cwd, sizeof(cwd));

    printf(FONT_CYAN "(%s)" FONT_GREEN " -> " FONT_COLOUR_RESET, cwd);
    fgets(line, 100, stdin);
    cmd = parse_command(line);
    free(line);
  } else {
    free_before_exit();
    exit(1);
  }
  int exit_code = run_command(cmd, builtins);

  delete_command(cmd);
  return exit_code;
}

void free_before_exit() {
  /***
   * Deallocate memory before exiting.
   *
   *
   */
  delete_command_list(command_queue);
}
