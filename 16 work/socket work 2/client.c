#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8080

int main() {
    int sock;
    struct sockaddr_in server_addr;
    char buffer[1024];
    char message[1024];

    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) {
        perror("socket");
        exit(EXIT_FAILURE);
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");

    if (connect(sock, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        perror("connect");
        exit(EXIT_FAILURE);
    }

    printf("Write messages to send:\n");

    while (1) {
        printf("You: ");
        fgets(message, 1024, stdin);
        send(sock, message, strlen(message), 0);

        memset(buffer, 0, sizeof(buffer));
        int bytes_received = recv(sock, buffer, 1024, 0);
        if (bytes_received > 0) {
            printf("Server: %s\n", buffer);
        }
    }

    close(sock);
    return 0;
}
