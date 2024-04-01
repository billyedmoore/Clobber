#include "header.h"

/***
 * Parsing inputs into Commands
 */

char *prompt() {
  /***
   * Display the prompt to the user and get the command.
   */
  char *line = malloc(MAX_LINE_SIZE * sizeof(char));
  char cwd[PATH_MAX];
  getcwd(cwd, sizeof(cwd));

  printf(FONT_CYAN "(%s)" FONT_GREEN " -> " FONT_COLOUR_RESET, cwd);
  fgets(line, MAX_LINE_SIZE, stdin);

  return line;
}

void populate_command_queue() {
  /***
   * Read lines from stdin attempting to parse each line and add it to the
   * queue.
   *
   * Output: Modifies the command_queue by adding the commands.
   *         Modifies the "stdin" by reading till it is empty.
   */
  char *line = malloc(100 * sizeof(char));
  while (fgets(line, 100, stdin) != NULL) {
    command_list cmds = parse_line(line);
    for (int i = 0; i < cmds.len; i++) {
      command_queue = append_command_list(command_queue, cmds.commands[i]);
    }
    delete_command_list(cmds);
  }
  free(line);
}

command_list parse_line(char *line) {
  /***
   * Parse a command into a "list" of arguments of type char**.
   * Allocates memory up to a bufsize.
   */
  line[strcspn(line, "\n")] = 0;
  int bufsize = 5; // The amount of pointers to allocate memory for.
  int i = 0;       // The current words being handled.
  char **args = malloc(sizeof(char *) *
                       bufsize); // Allocate the memory at inital bufsize.

  bool waiting_for_redirect_location = false;
  char *redirect_location = "";
  enum redirection_types redirect_type = NONE;

  // Loop through each word using strtok
  for (char *token = strtok(line, " "); token != NULL;
       token = strtok(NULL, " ")) {
    // If not enough memory has been allocated double the amount allocated.
    if (i + 1 <= bufsize) {
      bufsize = bufsize * 2;
      args = realloc(args, sizeof(char *) * bufsize);
    }
    // Handle special characters ">" and ">>", each allowed to consume only one
    // token a fd.
    if (strcmp(token, ">") == 0) {
      redirect_type = NORMAL;
      waiting_for_redirect_location = true;
    } else if (strcmp(token, ">>") == 0) {
      redirect_type = APPEND;
      waiting_for_redirect_location = true;
    } else if (waiting_for_redirect_location) {
      redirect_location = token;
      waiting_for_redirect_location = false;

    }
    // Set the next free pointer to the token;
    else {
      args[i] = token;
      i++;
    }
  }

  bool background = false;

  // If there is one or more arguments and the last is "&" run in background.
  if (i > 0 && strcmp(args[i - 1], "&") == 0) {
    background = true;
    i -= 1;
    args[i] = NULL; // remove "&" from the arguements.
  }
  args[i] = NULL;

  Command cmd = {copy_string_array(args, i), i, background, redirect_location,
                 NORMAL};

  command_list cmds = create_command_list();
  cmds = append_command_list(cmds, cmd);

  free(args);

  return cmds;
}

char **copy_string_array(char **source, int num_elements) {
  /***
   * Return a char** series of strings that is a copy of a char** series of
   * strings.
   */
  char **dest = malloc((num_elements + 1) * sizeof(char *));
  for (int i = 0; i < num_elements; i++) {
    size_t len = 1 + strlen(source[i]);
    dest[i] = malloc(len);
    strcpy(dest[i], source[i]);
  }
  dest[num_elements] = NULL;
  return dest;
}
