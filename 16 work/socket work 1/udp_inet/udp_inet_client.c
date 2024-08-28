#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>

#define SERVER_IP "127.0.0.1"
#define SERVER_PORT 12345
#define CLIENT_PORT 12346

void print_socket_address(int sockfd) {
  struct sockaddr_in addr;
  socklen_t addr_len = sizeof(addr);

  if (getsockname(sockfd, (struct sockaddr *)&addr, &addr_len) == -1) {
    perror("getsockname failed");
    return;
  }

  printf("Socket address: %s:%d\n", inet_ntoa(addr.sin_addr),ntohs(addr.sin_port));
}

int main() {
  int client_socket;
  struct sockaddr_in server_addr, client_addr;
  char buffer[1024];

  client_socket = socket(AF_INET, SOCK_DGRAM, 0);
  if (client_socket == -1) {
    perror("socket");
    exit(EXIT_FAILURE);
  }

  memset(&client_addr, 0, sizeof(client_addr));
  client_addr.sin_family = AF_INET;
  client_addr.sin_addr.s_addr = INADDR_ANY;
  client_addr.sin_port = htons(CLIENT_PORT);

  if (bind(client_socket, (struct sockaddr *)&client_addr,
           sizeof(client_addr)) == -1) {
    perror("Bind failed");
    close(client_socket);
    exit(EXIT_FAILURE);
  }

  print_socket_address(client_socket);

  memset(&server_addr, 0, sizeof(server_addr));
  server_addr.sin_family = AF_INET;
  server_addr.sin_addr.s_addr = inet_addr(SERVER_IP);
  server_addr.sin_port = htons(SERVER_PORT);

  while (1) {
    printf("Enter message to send: ");
    fgets(buffer, sizeof(buffer), stdin);
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
  return 0;
}
