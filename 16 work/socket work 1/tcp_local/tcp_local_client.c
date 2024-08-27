#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>

#define SOCKET_PATH "/tmp/tcp_local_socket10"

void print_peer_info(int socket_fd) {
  struct sockaddr_un addr;
  socklen_t len = sizeof(addr);

  if (getpeername(socket_fd, (struct sockaddr *)&addr, &len) == -1) {
    perror("getpeername failed");
    return;
  }

  printf("Connected to - Socket path: %s\n", addr.sun_path);
}

int main() {
  int client_socket;
  struct sockaddr_un server_addr;
  char buffer[1024];

  client_socket = socket(AF_LOCAL, SOCK_STREAM, 0);
  if (client_socket == -1) {
    perror("socket");
    exit(EXIT_FAILURE);
  }

  memset(&server_addr, 0, sizeof(server_addr));
  server_addr.sun_family = AF_LOCAL;
  strcpy(server_addr.sun_path, SOCKET_PATH);

  if (connect(client_socket, (struct sockaddr *)&server_addr,
              sizeof(server_addr)) == -1) {
    perror("Connect failed");
    close(client_socket);
    exit(EXIT_FAILURE);
  }

  print_peer_info(client_socket);

  while (1) {
    printf("Enter message to send: ");
    fgets(buffer, 1024, stdin);
    buffer[strcspn(buffer, "\n")] = 0;

    if (send(client_socket, buffer, strlen(buffer), 0) == -1) {
      perror("send failed");
      close(client_socket);
      exit(EXIT_FAILURE);
    }

    ssize_t recv_len = recv(client_socket, buffer, sizeof(buffer) - 1, 0);
    if (recv_len == -1) {
      perror("receive");
      close(client_socket);
      exit(EXIT_FAILURE);
    }

    buffer[recv_len] = '\0';
    printf("Modified message : %s\n", buffer);
  }

  close(client_socket);
  return 0;
}
