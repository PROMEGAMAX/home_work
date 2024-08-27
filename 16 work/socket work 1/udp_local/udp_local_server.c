#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>

#define SOCKET_PATH "/tmp/udp_local_socket"

void reverse_string(char *str) {
  int length = strlen(str);
  for (int i = 0; i < length / 2; i++) {
    char temp = str[i];
    str[i] = str[length - 1 - i];
    str[length - 1 - i] = temp;
  }
}

void print_socket_address(int sockfd) {
  struct sockaddr_un addr;
  socklen_t addr_len = sizeof(addr);

  if (getsockname(sockfd, (struct sockaddr *)&addr, &addr_len) == -1) {
    perror("getsockname failed");
    return;
  }

  printf("Socket address: %s\n", addr.sun_path);
}

int main() {
  int server_socket;
  struct sockaddr_un server_addr, client_addr;
  socklen_t client_addr_len = sizeof(client_addr);
  char buffer[1024];

  server_socket = socket(AF_LOCAL, SOCK_DGRAM, 0);
  if (server_socket == -1) {
    perror("Socket creation failed");
    exit(EXIT_FAILURE);
  }

  memset(&server_addr, 0, sizeof(server_addr));
  server_addr.sun_family = AF_LOCAL;
  strcpy(server_addr.sun_path, SOCKET_PATH);

  if (bind(server_socket, (struct sockaddr *)&server_addr,
           sizeof(server_addr)) == -1) {
    perror("Bind failed");
    close(server_socket);
    exit(EXIT_FAILURE);
  }

  print_socket_address(server_socket);

  printf("Server is running...\n");

  while (1) {
    ssize_t recv_len =
        recvfrom(server_socket, buffer, sizeof(buffer) - 1, 0,
                 (struct sockaddr *)&client_addr, &client_addr_len);
    if (recv_len == -1) {
      perror("Receive failed");
      close(server_socket);
      exit(EXIT_FAILURE);
    }

    buffer[recv_len] = '\0';
    printf("Received message: %s\n", buffer);

    reverse_string(buffer);

    if (sendto(server_socket, buffer, strlen(buffer), 0,
               (struct sockaddr *)&client_addr, client_addr_len) == -1) {
      perror("Send failed");
      close(server_socket);
      exit(EXIT_FAILURE);
    }
  }

  close(server_socket);
  unlink(SOCKET_PATH);
  return 0;
}
