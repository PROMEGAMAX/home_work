#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main() {
  sigset_t new_set, old_set;
  sigemptyset(&new_set);
  sigaddset(&new_set, SIGINT);

  if (sigprocmask(SIG_BLOCK, &new_set, &old_set) == -1) {
    perror("sigprocmask");
    exit(EXIT_FAILURE);
  }

  printf("Сигнал SIGINT заблокирован\n");
  while (1) {
    sleep(1);
  }

  return 0;
}