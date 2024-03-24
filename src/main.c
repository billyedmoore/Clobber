#include <stdio.h>
#include <string.h>
#include <unistd.h>

int run_command(char command[100]);

int main(int argc, char **argv) {
  if (isatty(STDIN_FILENO)) {
    char line[100];
    printf("Hey >");
    fgets(line, 100, stdin);
    run_command(line);
  } else {
    printf("We are in a script or are we?\n");
  }
}

int run_command(char command[100]) {
  /***
   * Parse and run a command.
   */

  // Create a temp copy of command as strcpy changes command.
  char temp_command[100];
  strcpy(temp_command, command);

  char *first_word = strtok(command, " ");

  strcpy(command, temp_command);
  printf("%s", command);
  return 1;
}
