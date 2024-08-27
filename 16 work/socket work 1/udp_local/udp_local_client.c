#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>

#define SOCKET_PATH "/tmp/udp_local_socket"
#define CLIENT_SOCKET_PATH "/tmp/udp_local_client_socket"

int main() {
  int client_socket;
  struct sockaddr_un server_addr, client_addr;
  char buffer[1024];

  client_socket = socket(AF_LOCAL, SOCK_DGRAM, 0);
  if (client_socket == -1) {
    perror("socket");
    exit(EXIT_FAILURE);
  }

  memset(&client_addr, 0, sizeof(client_addr));
  client_addr.sun_family = AF_LOCAL;
  strcpy(client_addr.sun_path, CLIENT_SOCKET_PATH);

  if (bind(client_socket, (struct sockaddr *)&client_addr,
           sizeof(client_addr)) == -1) {
    perror("Bind failed");
    close(client_socket);
    exit(EXIT_FAILURE);
  }

  memset(&server_addr, 0, sizeof(server_addr));
  server_addr.sun_family = AF_LOCAL;
  strcpy(server_addr.sun_path, SOCKET_PATH);

  while (1) {
    printf("Enter message to send: ");
    fgets(buffer, 1024, stdin);
    buffer[strcspn(buffer, "\n")] = 0;

    if (sendto(client_socket, buffer, strlen(buffer), 0,
               (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1) {
      perror("sendto");
      close(client_socket);
      exit(EXIT_FAILURE);
    }

    ssize_t recv_len =
        recvfrom(client_socket, buffer, sizeof(buffer) - 1, 0, NULL, NULL);
    if (recv_len == -1) {
      perror("receive");
      close(client_socket);
      exit(EXIT_FAILURE);
    }

    buffer[recv_len] = '\0';
    printf("Modified message : %s\n", buffer);
  }

  close(client_socket);
  unlink(CLIENT_SOCKET_PATH);
  return 0;
}
