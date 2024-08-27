#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>

#define SOCKET_PATH "/tmp/tcp_local_socket3"

void reverse_string(char *str) {
  int length = strlen(str);
  for (int i = 0; i < length / 2; i++) {
    char temp = str[i];
    str[i] = str[length - 1 - i];
    str[length - 1 - i] = temp;
  }
}

int main() {
  int server_socket, client_socket;
  struct sockaddr_un server_addr, client_addr;
  socklen_t client_addr_len = sizeof(client_addr);
  char buffer[1024];

  server_socket = socket(AF_LOCAL, SOCK_STREAM, 0);
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

  if(listen(server_socket,  3) == -1) {
    perror("Listen failed");
    close(server_socket);
    exit(EXIT_FAILURE);
  }

  printf("Server is running...\n");

  client_socket = accept(server_socket,  (struct sockaddr *)&client_addr, &client_addr_len);
  if(client_socket == -1){
    perror("Accept failed");
    close(server_socket);
    exit(EXIT_FAILURE);
  } 

  while (1) {
    ssize_t recv_len = recv(client_socket, buffer, sizeof(buffer) - 1, 0);
    if (recv_len == -1) {
      perror("Receive failed");
      close(client_socket);
      close(server_socket);
      exit(EXIT_FAILURE);
    } else if (recv == 0){
        printf("Client disconnect\n");
        break;  
    }

    buffer[recv_len] = '\0';
    printf("Received message: %s\n", buffer);

    reverse_string(buffer);

    if (send(client_socket, buffer, strlen(buffer), 0) == -1) {
      perror("Send failed");
      close(client_socket);
      close(server_socket);
      exit(EXIT_FAILURE);
    }
  }

  close(client_socket);
  close(server_socket);
  unlink(SOCKET_PATH);
  return 0;
}
