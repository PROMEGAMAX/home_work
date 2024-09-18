#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>    
#include <unistd.h> 
#include <string.h> 
#include <arpa/inet.h> 

#define PORT 8080 
#define MAX_CLIENTS 10 

void *handle_client(void *arg) {
    int client_sock = *(int*)arg;
    char buffer[1024];
    int bytes_received;

    while ((bytes_received = recv(client_sock, buffer, 1024, 0)) > 0) {
        printf("Received from client: %s\n", buffer);
        send(client_sock, buffer, strlen(buffer), 0);
    }

    close(client_sock);
    return NULL;
}

int main() {
    int server_sock, client_sock;
    struct sockaddr_in server_addr, client_addr;
    socklen_t addr_len = sizeof(client_addr);
    pthread_t thread_id;

    server_sock = socket(AF_INET, SOCK_STREAM, 0);
    if (server_sock < 0) {
        perror("socket");
        exit(EXIT_FAILURE);
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PORT);

    if (bind(server_sock, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        perror("Bind failed");
        close(server_sock);
        exit(EXIT_FAILURE);
    }

    if (listen(server_sock, MAX_CLIENTS) < 0) {
        perror("listen ");
        close(server_sock);     
        exit(EXIT_FAILURE);
    }

    while (1) {
        client_sock = accept(server_sock, (struct sockaddr*)&client_addr, &addr_len);
        if (client_sock < 0) {
            perror("accept");
            continue;
        }

        if (pthread_create(&thread_id, NULL, handle_client, (void*)&client_sock) != 0) {
            perror("create");
            close(client_sock);
        }
    }

    close(server_sock);
    return 0;
}
