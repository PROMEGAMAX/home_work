#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define MULTICAST_ADDR "239.0.0.1"
#define PORT 8080
#define BUFFER_SIZE 1024

int main(){
    struct sockaddr_in multicast_addr, local_addr;
    char buffer[BUFFER_SIZE];
    struct ip_mreq mreq;

    int sock_fd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sock_fd < 0) {
        perror("socket");
        exit(EXIT_FAILURE);
    }


    local_addr.sin_family = AF_INET;
    local_addr.sin_port = htons(PORT);
    local_addr.sin_addr.s_addr = INADDR_ANY;

    if(bind(sock_fd, (struct sockaddr *)&local_addr, sizeof(local_addr)) < 0){
        perror("bind");
        close(sock_fd);
        exit(EXIT_FAILURE);
    }


    mreq.imr_multiaddr.s_addr = inet_addr(MULTICAST_ADDR);
    mreq.imr_interface.s_addr = INADDR_ANY;

    if(setsockopt(sock_fd, IPPROTO_IP, IP_ADD_MEMBERSHIP,  &mreq, sizeof(mreq)) < 0){
        perror("setsockopt");
        close(sock_fd);
        exit(EXIT_FAILURE);
    }

    while(1){
        int byte_recieved = recvfrom(sock_fd, buffer,  BUFFER_SIZE, 0, NULL, NULL);
        if(byte_recieved < 0){
            perror("recvfrom");
            close(sock_fd);
            exit(EXIT_FAILURE);
            }

        printf("Recieved multicast message: %s\n",  buffer);
    }

    close(sock_fd);
    return 0;
}