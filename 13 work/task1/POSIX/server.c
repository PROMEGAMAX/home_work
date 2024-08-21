#include <fcntl.h>
#include <mqueue.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>

#define SERVER_QUEUE_NAME "/server_queue"
#define CLIENT_QUEUE_NAME "/client_queue"
#define MAX_SIZE 1024

int main() {
  mqd_t server_mq, client_mq;
  struct mq_attr attr;
  char buffer[MAX_SIZE];

  attr.mq_flags = 0;
  attr.mq_maxmsg = 10;
  attr.mq_msgsize = MAX_SIZE;
  attr.mq_curmsgs = 0;

  server_mq = mq_open(SERVER_QUEUE_NAME, O_CREAT | O_WRONLY, 0644, &attr);
  if (server_mq == (mqd_t)-1) {
    perror("Server: mq_open (server)");
    exit(1);
  }

  snprintf(buffer, MAX_SIZE, "Hi!");
  if (mq_send(server_mq, buffer, MAX_SIZE, 0) == -1) {
    perror("Server: mq_send");
    exit(1);
  }
  printf("Server: Sent message: %s\n", buffer);

  if (mq_close(server_mq) == -1) {
    perror("Server: mq_close (server)");
    exit(1);
  }

  client_mq = mq_open(CLIENT_QUEUE_NAME, O_CREAT | O_RDONLY, 0644, &attr);
  if (client_mq == (mqd_t)-1) {
    perror("Server: mq_open (client)");
    exit(1);
  }

  if (mq_receive(client_mq, buffer, MAX_SIZE, NULL) == -1) {
    perror("Server: mq_receive");
    exit(1);
  }
  printf("Server: Received message: %s\n", buffer);

  if (mq_close(client_mq) == -1) {
    perror("Server: mq_close (client)");
    exit(1);
  }

  if (mq_unlink(SERVER_QUEUE_NAME) == -1) {
    perror("Server: mq_unlink (server)");
    exit(1);
  }

  if (mq_unlink(CLIENT_QUEUE_NAME) == -1) {
    perror("Server: mq_unlink (client)");
    exit(1);
  }

  return 0;
}
