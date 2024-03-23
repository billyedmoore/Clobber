#include <stdio.h>
#include <unistd.h>

int main(int argc, char **argv) {
  if (isatty(STDIN_FILENO)) {
    char line[100];
    printf("Hey >");
    fgets(line, 100, stdin);
    printf("%s", line);
  } else {
    printf("We are in a script or are we?\n");
  }
}
