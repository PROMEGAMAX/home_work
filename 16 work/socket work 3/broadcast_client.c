#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 9090
#define BUFFER_SIZE 1024

int main(){
    char buffer[BUFFER_SIZE];
    struct sockaddr_in client_addr;
    int sock_fd = socket(AF_INET, SOCK_DGRAM, 0);

    if (sock_fd < 0) {
        perror("socket");
        exit(EXIT_FAILURE);
    }

    client_addr.sin_family = AF_INET;
    client_addr.sin_addr.s_addr = INADDR_ANY;
    client_addr.sin_port = htons(PORT);

    if (bind(sock_fd, (struct sockaddr *)&client_addr, sizeof(client_addr)) < 0) {
        perror("bind");
        close(sock_fd);
        exit(EXIT_FAILURE);
    }

    while (1) {
        socklen_t len = sizeof(client_addr);
        int bytes_received = recvfrom(sock_fd, buffer, BUFFER_SIZE, 0, (struct sockaddr *)&client_addr, &len);
        if (bytes_received < 0) {
            perror("recvfrom");
            close(sock_fd);
            exit(EXIT_FAILURE);
        }

        printf("Received broadcast message: %s\n", buffer);
    }

    close(sock_fd);
    return 0;
}
