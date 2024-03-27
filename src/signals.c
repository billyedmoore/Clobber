#include "header.h"
#include <signal.h>

void handle_sigchld() {
  /***
   * Catch SIGCHLD signal, when child stops executing.
   */
  // pid of the killed process.
  pid_t pid;
  // status of the child?
  int child_status;
  pid = waitpid(-1, &child_status, WNOHANG);

  for (int i; i < number_alive_background_processes; i++) {
    // If process is a background_process then print the kill message and remove
    // it from the list of background processes.
    if (background_processes[i] == pid) {
      printf("Process %lu Is Dead\n", (long)pid);
      number_alive_background_processes--;
      // shift the background_processes to fill the gap
      for (int j = i; j < number_alive_background_processes; j++) {
        background_processes[j] = background_processes[j + 1];
      }
      // set the last process id to zero
      background_processes[number_alive_background_processes] = 0;
      break;
    }
  }
}

void handle_sigint() {
  /***
   * Catch SIGINT signal.
   *
   * Pass CTRL-C to foreground process if it exists otherwise exit the program.
   */
  printf("%ld \n", (long)foreground_process);
  // Pass a SIGTERM aka CTRL-C to the child process
  if (foreground_process) {
    kill(foreground_process, SIGINT);
  } else {
    exit(1);
  }
}
