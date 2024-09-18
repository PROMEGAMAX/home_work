#include <stdio.h>
#include <stdlib.h>
#include <sys/epoll.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>

#define PORT 5050
#define MAX_EVENTS 10
#define BUFFER_SIZE 1024

int main() {
    int server_sock, client_sock, epoll_fd;
    struct sockaddr_in server_addr, client_addr;
    socklen_t addr_len = sizeof(client_addr);
    char buffer[BUFFER_SIZE];
    
    server_sock = socket(AF_INET, SOCK_STREAM, 0);
    if (server_sock < 0) {
        perror("socket");
        exit(EXIT_FAILURE);
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PORT);

    if (bind(server_sock, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        perror("bind");
        close(server_sock);
        exit(EXIT_FAILURE);
    }

    if (listen(server_sock, SOMAXCONN) < 0) {
        perror("listen");
        close(server_sock);
        exit(EXIT_FAILURE);
    }

    epoll_fd = epoll_create1(0);
    if (epoll_fd < 0) {
        perror("epoll_create");
        close(server_sock);
        exit(EXIT_FAILURE);
    }

    struct epoll_event event;
    event.events = EPOLLIN;
    event.data.fd = server_sock;
    if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, server_sock, &event) < 0) {
        perror("epoll_ctl");
        close(server_sock);
        close(epoll_fd);
        exit(EXIT_FAILURE);
    }

    struct epoll_event events[MAX_EVENTS];

    while (1) {
        int event_count = epoll_wait(epoll_fd, events, MAX_EVENTS, -1);
        if (event_count < 0) {
            perror("epoll_wait");
            break;
        }

        for (int i = 0; i < event_count; i++) {
            if (events[i].data.fd == server_sock) {
                client_sock = accept(server_sock, (struct sockaddr*)&client_addr, &addr_len);
                if (client_sock < 0) {
                    perror("accept");
                    continue;
                }

                event.events = EPOLLIN;
                event.data.fd = client_sock;
                if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, client_sock, &event) < 0) {
                    perror("epoll_ctl (client)");
                    close(client_sock);
                }
            } 
            else {
                int client_sock = events[i].data.fd;
                int bytes_received = recv(client_sock, buffer, BUFFER_SIZE, 0);
                
                if (bytes_received <= 0) {
                    close(client_sock);
                } else {
                    send(client_sock, buffer, bytes_received, 0);
                }
            }
        }
    }

    close(server_sock);
    close(epoll_fd);
    return 0;
}
