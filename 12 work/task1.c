#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>

int main() {
  int p[2];
  pid_t pid;
  char buff[80];

  if (pipe(p)) {
    perror("Ожибка канала");
    exit(1);
  }
  if ((pid = fork()) < 0) {
    perror("Ошибка fork");
  } else if (pid == 0) {
    close(p[1]);
    read(p[0], buff, sizeof(buff));
    printf("%s\n", buff);
    close(p[0]);
    exit(0);
  } else {
    close(p[0]);
    write(p[1], "Hi!", 4);
    close(p[1]);
  }
  return 0;
}