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

  server_msgid = msgget(SERVER_KEY, IPC_CREAT | 0666);
  if (server_msgid == -1) {
    perror("Server: msgget (server)");
    exit(1);
  }

  msg.msg_type = 1;
  snprintf(msg.msg_text, MAX_SIZE, "Hi!");
  if (msgsnd(server_msgid, &msg, sizeof(msg.msg_text), 0) == -1) {
    perror("Server: msgsnd");
    exit(1);
  }
  printf("Server: Sent message: %s\n", msg.msg_text);

  client_msgid = msgget(CLIENT_KEY, IPC_CREAT | 0666);
  if (client_msgid == -1) {
    perror("Server: msgget (client)");
    exit(1);
  }

  if (msgrcv(client_msgid, &msg, sizeof(msg.msg_text), 1, 0) == -1) {
    perror("Server: msgrcv");
    exit(1);
  }
  printf("Server: Received message: %s\n", msg.msg_text);

  if (msgctl(server_msgid, IPC_RMID, NULL) == -1) {
    perror("Server: msgctl (server)");
    exit(1);
  }

  if (msgctl(client_msgid, IPC_RMID, NULL) == -1) {
    perror("Server: msgctl (client)");
    exit(1);
  }

  return 0;
}
