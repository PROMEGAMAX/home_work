#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>

int main() {
  char *fifo_name = "/tmp/my_fifo";
  int fifo_fd;
  char buffer[1024];

  fifo_fd = open(fifo_name, O_RDONLY);
  if (fifo_fd < 0) {
    perror("open failed");
    exit(1);
  }
  ssize_t bytes_read = read(fifo_fd, buffer, 1024);
  if (bytes_read < 0) {
    perror("read failed");
    exit(1);
  }
  buffer[bytes_read] = '\0';
  printf("%s\n", buffer);
  close(fifo_fd);

  if (unlink(fifo_name) < 0) {
    perror("unlink failed");
    exit(1);
  }
  
  return 0;
}