#include "header.h"

/***
 * Parsing inputs into Commands
 */

Command parse_command(char *line) {
  /***
   * Parse a command into a "list" of arguments of type char**.
   * Allocates memory up to a bufsize.
   */
  line[strcspn(line, "\n")] = 0;
  int bufsize = 5; // The amount of pointers to allocate memory for.
  int i = 0;       // The current words being handled.
  char **args = malloc(sizeof(char *) *
                       bufsize); // Allocate the memory at inital bufsize.

  // Loop through each word using strtok
  for (char *token = strtok(line, " "); token != NULL;
       token = strtok(NULL, " ")) {
    // If not enough memory has been allocated double the amount allocated.
    if (i <= bufsize) {
      bufsize = bufsize * 2;
      args = realloc(args, sizeof(char *) * bufsize);
    }
    // Assign the word (token) to the next free pointer.
    args[i] = token;
    i++;
  }

  bool background = false;

  // If there is one or more arguments and the last is "&" run in background.
  if (i > 0 && strcmp(args[i - 1], "&") == 0) {
    background = true;
    i -= 1;
    args[i] = NULL; // remove "&" from the arguements.
  }
  // Reallocate memory so only the amount needed is allocated.
  args = realloc(args, (i) * sizeof(char *));
  Command cmd = {args, i, background};

  return cmd;
}
