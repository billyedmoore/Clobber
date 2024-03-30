#include "header.h"

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
  pid_t pid;
  // If more processes in background than supported dont fork.
  if (number_alive_background_processes >= MAX_BACKGROUND) {
    pid = -2;
  } else {
    pid = fork();
  }
  switch (pid) {
  case -1:
    // Failed to fork
    printf("Couldn't fork.\n");
    return 1;
  case -2:
    // Too many background jobs
    printf("Too many background jobs.\n");
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
      // Keep track of foreground process so signals can be attributed.
      foreground_process = pid;
      wait(NULL);
      foreground_process = 0;
    } else {
      printf("Process %lu Lives\n", (long)pid);
      background_processes[number_alive_background_processes] = pid;
      number_alive_background_processes++;
    }
    return 0;
  }

  // If we haven't run a command then command not found.
  printf("Command \"%s\" not found.\n", cmd.arguments[0]);

  return 1;
}
