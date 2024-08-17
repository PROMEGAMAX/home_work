#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>

int main() {
  char *fifo_name = "/tmp/my_fifo";
  int fifo_fd;

  if (mkfifo(fifo_name, 0666) < 0) {
    perror("mkfifo failed");
    exit(1);
  }
  fifo_fd = open(fifo_name, O_WRONLY);
  if (fifo_fd < 0) {
    perror("open failed");
    exit(1);
  }
  char *message = "Hi!";
  write(fifo_fd, message, strlen(message));
  close(fifo_fd);
  
  return 0;
}