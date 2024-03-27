#include "header.h"

void handle_sigchld() {
  /***
   * Catch SIGCHLD signal, when child stops executing.
   * Write the fact it has exited to STDOUT.
   *
   * Input: Will have to get back to you on that one?.
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
