#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include <netinet/in.h>

#define SERVER_IP "127.0.0.1"
#define SERVER_PORT 12345
#define CLIENT_PORT 12346

struct udphdr {
    uint16_t source; 
    uint16_t dest;   
    uint16_t length;  
    uint16_t check;   
};

int main() {
    int client_socket;
    struct sockaddr_in server_addr;
    char buffer[1024];
    struct udphdr udp_header;

    client_socket = socket(AF_INET, SOCK_RAW, IPPROTO_UDP);
    if (client_socket == -1) {
        perror("socket");
        exit(EXIT_FAILURE);
    }

    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = inet_addr(SERVER_IP);  
    server_addr.sin_port = htons(SERVER_PORT);  

    while (1) {
        printf("Enter message to send: ");
        fgets(buffer, sizeof(buffer), stdin);
        buffer[strcspn(buffer, "\n")] = 0;

        udp_header.source = htons(CLIENT_PORT);  
        udp_header.dest = htons(SERVER_PORT);    
        udp_header.length = htons(8 + strlen(buffer));  
        udp_header.check = 0;  

        char packet[1024];
        memset(packet, 0, sizeof(packet));
        memcpy(packet, &udp_header, sizeof(udp_header));  
        memcpy(packet + sizeof(udp_header), buffer, strlen(buffer)); 

        ssize_t packet_len = sizeof(udp_header) + strlen(buffer);
        if (sendto(client_socket, packet, packet_len, 0,
                   (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1) {
            perror("sendto");
            close(client_socket);
            exit(EXIT_FAILURE);
        }

        ssize_t recv_len = recvfrom(client_socket, buffer, sizeof(buffer) - 1, 0, NULL, NULL);
        if (recv_len == -1) {
            perror("receive");
            close(client_socket);
            exit(EXIT_FAILURE);
        }

        buffer[recv_len] = '\0';
        printf("Modified message : %s\n", buffer);
        printf("Sending to port: %d\n", ntohs(udp_header.dest));
        printf("Client port: %d\n", ntohs(udp_header.source));
    }
    close(client_socket);
    return 0;
}
