#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 9090
#define BUFFER_SIZE 1024

int main(){
    char *message = "BROADCAST MESSAGE!!!";
    struct sockaddr_in broadcast_addr;
    int broadcast_enable = 1;

    int sock_fd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sock_fd < 0) {
        perror("socket");
        exit(EXIT_FAILURE);
    }

    if (setsockopt(sock_fd, SOL_SOCKET, SO_BROADCAST, &broadcast_enable, sizeof(broadcast_enable)) < 0) {
        perror("setsockopt");
        close(sock_fd);
        exit(EXIT_FAILURE);
    }

    broadcast_addr.sin_family = AF_INET;
    broadcast_addr.sin_addr.s_addr = inet_addr("255.255.255.255");
    broadcast_addr.sin_port = htons(PORT);

    while (1) {
        if (sendto(sock_fd, message, strlen(message), 0, (struct sockaddr *)&broadcast_addr, sizeof(broadcast_addr)) < 0) {
            perror("sendto");
            close(sock_fd);
            exit(EXIT_FAILURE);
        }

        printf("Broadcast message sent: %s\n", message);
        sleep(5);
    }

    close(sock_fd);
    return 0;
}
