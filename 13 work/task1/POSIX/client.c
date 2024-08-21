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
  char buffer[MAX_SIZE];

  server_mq = mq_open(SERVER_QUEUE_NAME, O_RDONLY);
  if (server_mq == (mqd_t)-1) {
    perror("Client: mq_open (server)");
    exit(1);
  }

  if (mq_receive(server_mq, buffer, MAX_SIZE, NULL) == -1) {
    perror("Client: mq_receive");
    exit(1);
  }
  printf("Client: Received message: %s\n", buffer);

  if (mq_close(server_mq) == -1) {
    perror("Client: mq_close (server)");
    exit(1);
  }

  client_mq = mq_open(CLIENT_QUEUE_NAME, O_WRONLY);
  if (client_mq == (mqd_t)-1) {
    perror("Client: mq_open (client)");
    exit(1);
  }

  snprintf(buffer, MAX_SIZE, "Hello!");
  if (mq_send(client_mq, buffer, MAX_SIZE, 0) == -1) {
    perror("Client: mq_send");
    exit(1);
  }
  printf("Client: Sent message: %s\n", buffer);

  if (mq_close(client_mq) == -1) {
    perror("Client: mq_close (client)");
    exit(1);
  }

  return 0;
}
