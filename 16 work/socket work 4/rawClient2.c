#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include <netinet/ip.h>
#include <netinet/udp.h>  
#include <errno.h>

#define SERVER_IP "127.0.0.1"
#define SERVER_PORT 12342
#define CLIENT_PORT 12346

struct pseudo_header {
    uint32_t src_addr;
    uint32_t dst_addr;
    uint8_t zero;
    uint8_t protocol;
    uint16_t udp_length;
} __attribute__((packed));

uint16_t checksum(void *buf, int len) {
    uint16_t *data = buf;
    unsigned long sum = 0;
    while (len > 1) {
        sum += *data++;
        len -= 2;
    }
    if (len == 1) {
        sum += *(uint8_t*)data;
    }
    while (sum >> 16)
        sum = (sum & 0xFFFF) + (sum >> 16);
    return (uint16_t)(~sum);
}

int main() {
    int client_socket;
    struct sockaddr_in server_addr;
    char data[] = "privet servak";
    struct udphdr udp_header;
    char packet[1024];
    
    client_socket = socket(AF_INET, SOCK_RAW, IPPROTO_RAW);
    if (client_socket == -1) {
        perror("socket");
        exit(EXIT_FAILURE);
    }

    int one = 1;
    if (setsockopt(client_socket, IPPROTO_IP, IP_HDRINCL, &one, sizeof(one)) < 0) {
        perror("setsockopt");
        close(client_socket);
        exit(EXIT_FAILURE);
    }

    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = inet_addr(SERVER_IP);
    server_addr.sin_port = htons(SERVER_PORT);

    udp_header.source = htons(CLIENT_PORT);
    udp_header.dest = htons(SERVER_PORT);
    udp_header.len = htons(sizeof(struct udphdr) + strlen(data));
    udp_header.check = 0;

    struct iphdr ip_header;
    ip_header.version = 4;              
    ip_header.ihl = 5;                    
    ip_header.tos = 0;                   
    ip_header.tot_len = htons(sizeof(struct iphdr) + sizeof(struct udphdr) + strlen(data));
    ip_header.id = htons(54321);
    ip_header.frag_off = 0;
    ip_header.ttl = 255;                
    ip_header.protocol = IPPROTO_UDP;    
    ip_header.check = 0;                
    ip_header.saddr = inet_addr(SERVER_IP); 
    ip_header.daddr = inet_addr(SERVER_IP);

    ip_header.check = checksum(&ip_header, sizeof(ip_header));

    struct pseudo_header psh;
    psh.src_addr = ip_header.saddr;
    psh.dst_addr = ip_header.daddr;
    psh.zero = 0;
    psh.protocol = IPPROTO_UDP;
    psh.udp_length = udp_header.len;

    int psize = sizeof(struct pseudo_header) + sizeof(struct udphdr) + strlen(data);
    char *pseudogram = malloc(psize);
    if (pseudogram == NULL) {
        perror("malloc");
        close(client_socket);
        exit(EXIT_FAILURE);
    }

    memcpy(pseudogram, &psh, sizeof(struct pseudo_header));
    memcpy(pseudogram + sizeof(struct pseudo_header), &udp_header, sizeof(struct udphdr));
    memcpy(pseudogram + sizeof(struct pseudo_header) + sizeof(struct udphdr), data, strlen(data));

    udp_header.check = checksum(pseudogram, psize);

    free(pseudogram);

    memset(packet, 0, sizeof(packet));
    memcpy(packet, &ip_header, sizeof(struct iphdr));
    memcpy(packet + sizeof(struct iphdr), &udp_header, sizeof(struct udphdr));
    memcpy(packet + sizeof(struct iphdr) + sizeof(struct udphdr), data, strlen(data));


    ssize_t packet_len = sizeof(struct iphdr) + sizeof(struct udphdr) + strlen(data);
    if (sendto(client_socket, packet, packet_len, 0,
               (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1) {
        perror("sendto");
        close(client_socket);
        exit(EXIT_FAILURE);
    }

    printf("Пакет успешно отправлен\n");

    close(client_socket);
    return 0;
}
