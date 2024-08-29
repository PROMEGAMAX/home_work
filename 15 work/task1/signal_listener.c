#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

void listener(int sig) { printf("Получен сигнал SIGUSR1\n"); }

int main() {
  struct sigaction act;

  act.sa_handler = listener;
  act.sa_flags = 0;
  sigemptyset(&act.sa_mask);

  if (sigaction(SIGUSR1, &act, NULL) == -1) {
    perror("sigaction");
    exit(EXIT_FAILURE);
  }

  while (1) {
    sleep(1);
  }

  return 0;
}
