#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

#define SERVER_PORT 12345

void reverse_string(char *str) {
  int length = strlen(str);
  for (int i = 0; i < length / 2; i++) {
    char temp = str[i];
    str[i] = str[length - 1 - i];
    str[length - 1 - i] = temp;
  }
}

void print_socket_address(int sockfd) {
  struct sockaddr_in addr;
  socklen_t addr_len = sizeof(addr);

  if (getsockname(sockfd, (struct sockaddr *)&addr, &addr_len) == -1) {
    perror("getsockname failed");
    return;
  }

  printf("Socket address: %s:%d\n", inet_ntoa(addr.sin_addr),
         ntohs(addr.sin_port));
}

int main() {
  int server_socket, client_socket;
  struct sockaddr_in server_addr, client_addr;
  socklen_t client_addr_len = sizeof(client_addr);
  char buffer[1024];

  server_socket = socket(AF_INET, SOCK_STREAM, 0);
  if (server_socket == -1) {
    perror("Socket creation failed");
    exit(EXIT_FAILURE);
  }

  memset(&server_addr, 0, sizeof(server_addr));
  server_addr.sin_family = AF_INET;
  server_addr.sin_addr.s_addr = INADDR_ANY;
  server_addr.sin_port = htons(SERVER_PORT);

  if (bind(server_socket, (struct sockaddr *)&server_addr,
           sizeof(server_addr)) == -1) {
    perror("Bind failed");
    close(server_socket);
    exit(EXIT_FAILURE);
  }

  print_socket_address(server_socket);

  if (listen(server_socket, 5) == -1) {
    perror("Listen failed");
    close(server_socket);
    exit(EXIT_FAILURE);
  }

  printf("Server is running...\n");

  while (1) {
    client_socket = accept(server_socket, (struct sockaddr *)&client_addr,
                           &client_addr_len);
    if (client_socket == -1) {
      perror("Accept failed");
      close(server_socket);
      exit(EXIT_FAILURE);
    }

    printf("Connected to client\n");

    while (1) {
      ssize_t recv_len = recv(client_socket, buffer, sizeof(buffer) - 1, 0);
      if (recv_len == 0) {
        printf("Client disconnected\n");
        break;
      } else if (recv_len == -1) {
        perror("Receive failed");
        break;
      }

      buffer[recv_len] = '\0';
      printf("Received message: %s\n", buffer);

      reverse_string(buffer);

      if (send(client_socket, buffer, strlen(buffer), 0) == -1) {
        perror("Send failed");
        break;
      }
    }

    close(client_socket);
  }

  close(server_socket);
  return 0;
}
