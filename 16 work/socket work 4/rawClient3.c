#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include <netinet/ip.h>
#include <netinet/udp.h>
#include <netinet/ether.h>
#include <sys/ioctl.h>
#include <net/if.h>
#include <linux/if_packet.h>
#include <errno.h>

#define SERVER_IP "192.168.1.3"         
#define SERVER_MAC "a0:d7:68:60:5b:b2" 
#define SERVER_PORT 12342             
#define CLIENT_IP "192.168.1.2"          
#define INTERFACE "wlxa0d768605bb2"

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
    int sockfd;
    char buffer[1024];
    struct ethhdr *eth_header;
    struct iphdr *ip_header;
    struct udphdr *udp_header;
    char *data;

    sockfd = socket(AF_PACKET, SOCK_RAW, htons(ETH_P_ALL));
    if (sockfd < 0) {
        perror("socket");
        exit(EXIT_FAILURE);
    }

    struct ifreq ifr;
    memset(&ifr, 0, sizeof(struct ifreq));
    strncpy(ifr.ifr_name, INTERFACE, IFNAMSIZ - 1);

    if (ioctl(sockfd, SIOCGIFHWADDR, &ifr) == -1) {
        perror("SIOCGIFHWADDR");
        close(sockfd);
        exit(EXIT_FAILURE);
    }

    unsigned char src_mac[6];
    memcpy(src_mac, ifr.ifr_hwaddr.sa_data, 6);

    if (ioctl(sockfd, SIOCGIFINDEX, &ifr) == -1) {
        perror("SIOCGIFINDEX");
        close(sockfd);
        exit(EXIT_FAILURE);
    }

    unsigned char dest_mac[6];
    sscanf(SERVER_MAC, "%hhx:%hhx:%hhx:%hhx:%hhx:%hhx",
           &dest_mac[0], &dest_mac[1], &dest_mac[2],
           &dest_mac[3], &dest_mac[4], &dest_mac[5]);


    eth_header = (struct ethhdr *)buffer;
    memcpy(eth_header->h_dest, dest_mac, 6);    
    memcpy(eth_header->h_source, src_mac, 6);  
    eth_header->h_proto = htons(ETH_P_IP);   


    ip_header = (struct iphdr *)(buffer + sizeof(struct ethhdr));
    ip_header->version = 4;                   
    ip_header->ihl = 5;                  
    ip_header->tos = 0;                   
    ip_header->tot_len = htons(sizeof(struct iphdr) + sizeof(struct udphdr) + strlen("privet servak"));
    ip_header->id = htons(54321);         
    ip_header->frag_off = 0;                 
    ip_header->ttl = 64;                    
    ip_header->protocol = IPPROTO_UDP;        
    ip_header->saddr = inet_addr(CLIENT_IP);    
    ip_header->daddr = inet_addr(SERVER_IP);    
    ip_header->check = 0;                    
    ip_header->check = checksum(ip_header, sizeof(struct iphdr));


    udp_header = (struct udphdr *)(buffer + sizeof(struct ethhdr) + sizeof(struct iphdr));
    udp_header->source = htons(SERVER_PORT);    
    udp_header->dest = htons(SERVER_PORT);
    udp_header->len = htons(sizeof(struct udphdr) + strlen("privet servak"));
    udp_header->check = 0;         


    data = buffer + sizeof(struct ethhdr) + sizeof(struct iphdr) + sizeof(struct udphdr);
    strcpy(data, "privet servak");

    struct pseudo_header psh;
    psh.src_addr = ip_header->saddr;
    psh.dst_addr = ip_header->daddr;
    psh.zero = 0;
    psh.protocol = IPPROTO_UDP;
    psh.udp_length = udp_header->len;

    int psize = sizeof(struct pseudo_header) + sizeof(struct udphdr) + strlen("privet servak");
    char *pseudogram = malloc(psize);
    if (pseudogram == NULL) {
        perror("malloc");
        close(sockfd);
        exit(EXIT_FAILURE);
    }

    memcpy(pseudogram, &psh, sizeof(struct pseudo_header));
    memcpy(pseudogram + sizeof(struct pseudo_header), udp_header, sizeof(struct udphdr));
    memcpy(pseudogram + sizeof(struct pseudo_header) + sizeof(struct udphdr), data, strlen("privet servak"));

    udp_header->check = checksum(pseudogram, psize);

    free(pseudogram);

    ssize_t packet_len = sizeof(struct ethhdr) + sizeof(struct iphdr) + sizeof(struct udphdr) + strlen("privet servak");

    struct sockaddr_ll socket_address;
    memset(&socket_address, 0, sizeof(struct sockaddr_ll));
    socket_address.sll_ifindex = ifr.ifr_ifindex;
    socket_address.sll_halen = ETH_ALEN;
    memcpy(socket_address.sll_addr, dest_mac, 6);

    if (sendto(sockfd, buffer, packet_len, 0, (struct sockaddr*)&socket_address, sizeof(struct sockaddr_ll)) == -1) {
        perror("sendto");
        close(sockfd);
        exit(EXIT_FAILURE);
    }

    printf("Пакет успешно отправлен\n");

    close(sockfd);
    return 0;
}
