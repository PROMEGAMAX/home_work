#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/msg.h>

#define SERVER_KEY 1234
#define CLIENT_KEY 5678
#define MAX_SIZE 1024

struct message {
  long msg_type;
  char msg_text[MAX_SIZE];
};

int main() {
  int server_msgid, client_msgid;
  struct message msg;

  server_msgid = msgget(SERVER_KEY, 0666);
  if (server_msgid == -1) {
    perror("Client: msgget (server)");
    exit(1);
  }

  if (msgrcv(server_msgid, &msg, sizeof(msg.msg_text), 1, 0) == -1) {
    perror("Client: msgrcv");
    exit(1);
  }
  printf("Client: Received message: %s\n", msg.msg_text);

  client_msgid = msgget(CLIENT_KEY, 0666);
  if (client_msgid == -1) {
    perror("Client: msgget (client)");
    exit(1);
  }

  msg.msg_type = 1;
  snprintf(msg.msg_text, MAX_SIZE, "Hello!");
  if (msgsnd(client_msgid, &msg, sizeof(msg.msg_text), 0) == -1) {
    perror("Client: msgsnd");
    exit(1);
  }
  printf("Client: Sent message: %s\n", msg.msg_text);

  return 0;
}
