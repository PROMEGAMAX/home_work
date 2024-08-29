#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
  if (argc != 2) {
    printf("Использование PID :  %s <PID>\n", argv[0]);
    exit(EXIT_FAILURE);
  }

  pid_t pid = atoi(argv[1]);

  if (kill(pid, SIGUSR1) == -1) {
    perror("kill");
    exit(EXIT_FAILURE);
  }

  printf("Сигнал отправлен процессу с PID :  %d\n", pid);

  return 0;
}