#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define MULTICAST_ADDR "239.0.0.1"
#define PORT 8080
#define BUFFER_SIZE 1024

int main(){
    struct sockaddr_in  multicast_addr;
    char *message = "Multicast message!!!";
    struct ip_mreq mreq;

    int sock_fd = socket(AF_INET, SOCK_DGRAM, 0);
    if(sock_fd < 0){
        perror("socket");
        exit(EXIT_FAILURE);
    }

    multicast_addr.sin_family = AF_INET;
    multicast_addr.sin_port = htons(PORT);
    multicast_addr.sin_addr.s_addr = inet_addr(MULTICAST_ADDR);

    if(sendto(sock_fd, message, strlen(message), 0, (struct sockaddr *)&multicast_addr, sizeof(multicast_addr)) < 0){
    perror("sendto");
    exit(EXIT_FAILURE);
    }

    printf("Multicast  message sent\n");

    close(sock_fd);
    return 0;
}