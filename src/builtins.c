#include "header.h"

/***
 * Functions for builtin commands.
 */

built_in_func *create_builtins() {
  built_in_func *builtins_l = malloc(NUMBER_BUILTINS * sizeof(built_in_func));

  // Define the builtins.
  builtins_l[0].func = &builtin_cd;
  builtins_l[0].name = "cd";

  builtins_l[1].func = &builtin_exit;
  builtins_l[1].name = "exit";

  builtins_l[2].func = &builtin_help;
  builtins_l[2].name = "help";

  return builtins_l;
}

int builtin_cd(Command cmd) {
  /***
   * Builtin "cd" command to change the current working directory.
   *
   * Input:
   *  Command cmd: The command made up of the argumentss (including "cd" as
   * the first value) and the count. Output: int exit_code : See run_command
   * for details.
   */
  if (cmd.count <= 1) {
    printf("Not enough arguments silly.\n");
    return 2;
  } else {
    int changed = chdir(cmd.arguments[1]);

    switch (changed) {
    case 0:
      return 0;
    case -1:
      return 1;
    default:
      return 1;
    }
  }
}

int builtin_exit(Command cmd) {
  /***
   * Builtin "exit" command.
   */
  pending_exit = true;
  return 1;
}

int builtin_help(Command cmd) {
  /***
   * Builtin "help" command.
   * Input:
   *  Command cmd: Not yet used, may be used to parse arguments in the future.
   * Output:
   *  int exit_code see execute command for details.
   */
  printf("Welcome to Clobber.\n");
  printf("Like any shell you can run anything in your path.\n");
  printf("There are three builtins \"cd\" \"help\" \"exit\".\n");
  printf("Append & to any command to have it run in the background.\n");
  printf("CTRL-C will kill the running process if there is one, if not it will "
         "kill Clobber.\n");
  printf("Hoping to add redirection and piping in the future.\n");
  printf("This is an educational exercise so it is pretty simple and usability "
         "is limited.\n");

  return 0;
}
