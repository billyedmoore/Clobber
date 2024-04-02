#include "header.h"
#include <stdlib.h>
#include <string.h>

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
  char *line = malloc(MAX_LINE_SIZE * sizeof(char));
  while (fgets(line, MAX_LINE_SIZE, stdin) != NULL) {
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

  // delete new line symbol breaks if newline not at the end of string..
  line[strcspn(line, "\n")] = '\0';

  command_list cmds = create_command_list();
  split_line sl = split_on_symbol(line, '|');

  for (int i = 0; i < sl.count; i++) {
    Command cmd = parse_one_command(sl.splits[i]);
    cmds = append_command_list(cmds, cmd);
  }
  delete_split_line(sl);

  return cmds;
}

split_line create_split_line() {
  char **splits = calloc(BUFFER_SIZE, sizeof(char *));
  split_line sl = {splits, 0, BUFFER_SIZE};
  return sl;
}

split_line append_split(split_line sl, char *token) {
  if (sl.count == sl.allocated_size) {
    sl.allocated_size = sl.allocated_size * 2;
    sl.splits = realloc(sl.splits, sl.allocated_size);
  }
  sl.splits[sl.count] = token;
  sl.count++;
  return sl;
}

void delete_split_line(split_line sl) {
  for (int i = 0; i < sl.count; i++) {
    free(sl.splits[i]);
  }
  free(sl.splits);
}

split_line split_on_symbol(char *line, char sym) {
  /***
   * Implementing this without strtok
   */

  size_t string_len = 0;

  while (1) {
    // if reached end of string
    if (line[string_len] == '\0') {
      break;
    }

    string_len++;
  }
  size_t prev_token_pos = 0;

  split_line sl = create_split_line();

  for (int i = 0; i <= string_len; i++) {
    if (line[i] == sym || i == string_len) {
      line[i] = '\0';
      int token_len = i - prev_token_pos;
      char *token = malloc((token_len + 1) * sizeof(char));
      strncpy(token, line + prev_token_pos, token_len + 1);
      prev_token_pos = i + 1;

      sl = append_split(sl, token);
    }
  }
  return sl;
}

Command parse_one_command(char *line) {
  // copy line so that the function
  char *line_cpy = strdup(line);
  int bufsize = BUFFER_SIZE; // The amount of pointers to allocate memory for.
  // Parse one command from a string into a Command object.
  int i = 0; // The index of the words being handled.
  char **args = malloc(sizeof(char *) *
                       bufsize); // Allocate the memory at inital bufsize.

  bool waiting_for_redirect_location = false;
  char *redirect_location = "";
  enum redirection_types redirect_type = NONE;

  // Loop through each word using strtok
  for (char *token = strtok(line_cpy, " "); token != NULL;
       token = strtok(NULL, " ")) {
    // If not enough memory has been allocated double the amount allocated.
    if (i + 1 <= bufsize) {
      bufsize = bufsize * 2;
      args = realloc(args, sizeof(char *) * bufsize);
    }
    // Handle special characters ">" and ">>", each allowed to consume only
    // one token a fd.
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

  Command cmd = create_command(copy_string_array(args, i), i, background,
                               redirect_location, NORMAL, false);
  free(args);
  free(line_cpy);
  return cmd;
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
