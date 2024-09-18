#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/select.h>
#include <sys/socket.h>
#include <netinet/in.h>

#define TCP_PORT 3030
#define UDP_PORT 2020
#define BUFFER_SIZE 1024

int main() {
    int tcp_socket, udp_socket, max_sd;
    struct sockaddr_in tcp_addr, udp_addr;
    fd_set read_fds;
    char buffer[BUFFER_SIZE];

    // Создаем TCP-сокет
    tcp_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (tcp_socket < 0) {
        perror("TCP socket creation failed");
        exit(EXIT_FAILURE);
    }

    // Настраиваем адрес для TCP-сокета
    tcp_addr.sin_family = AF_INET;
    tcp_addr.sin_addr.s_addr = INADDR_ANY;
    tcp_addr.sin_port = htons(TCP_PORT);

    // Привязываем TCP-сокет к адресу
    if (bind(tcp_socket, (struct sockaddr *)&tcp_addr, sizeof(tcp_addr)) < 0) {
        perror("TCP bind failed");
        close(tcp_socket);
        exit(EXIT_FAILURE);
    }

    // Начинаем слушать на TCP-сокете
    if (listen(tcp_socket, 5) < 0) {
        perror("TCP listen failed");
        close(tcp_socket);
        exit(EXIT_FAILURE);
    }

    // Создаем UDP-сокет
    udp_socket = socket(AF_INET, SOCK_DGRAM, 0);
    if (udp_socket < 0) {
        perror("UDP socket creation failed");
        close(tcp_socket);
        exit(EXIT_FAILURE);
    }

    // Настраиваем адрес для UDP-сокета
    udp_addr.sin_family = AF_INET;
    udp_addr.sin_addr.s_addr = INADDR_ANY;
    udp_addr.sin_port = htons(UDP_PORT);

    // Привязываем UDP-сокет к адресу
    if (bind(udp_socket, (struct sockaddr *)&udp_addr, sizeof(udp_addr)) < 0) {
        perror("UDP bind failed");
        close(tcp_socket);
        close(udp_socket);
        exit(EXIT_FAILURE);
    }

    printf("Server is listening on TCP port %d and UDP port %d\n", TCP_PORT, UDP_PORT);

    while (1) {
        // Очищаем набор дескрипторов
        FD_ZERO(&read_fds);

        // Добавляем TCP и UDP сокеты в набор дескрипторов
        FD_SET(tcp_socket, &read_fds);
        FD_SET(udp_socket, &read_fds);

        // Определяем максимальный дескриптор для select
        if (tcp_socket > udp_socket) {
            max_sd = tcp_socket;
        } else {
            max_sd = udp_socket;
        }

        // Ожидаем события на одном из сокетов
        int activity = select(max_sd + 1, &read_fds, NULL, NULL, NULL);
        if (activity < 0) {
            perror("select error");
            close(tcp_socket);
            close(udp_socket);
            exit(EXIT_FAILURE);
        }

        // Проверяем, если на TCP-сокете есть новое подключение
        if (FD_ISSET(tcp_socket, &read_fds)) {
            int new_socket, addr_len = sizeof(tcp_addr);
            new_socket = accept(tcp_socket, (struct sockaddr *)&tcp_addr, (socklen_t *)&addr_len);
            if (new_socket < 0) {
                perror("accept error");
                close(tcp_socket);
                close(udp_socket);
                exit(EXIT_FAILURE);
            }

            // Чтение данных от TCP-клиента
            memset(buffer, 0, BUFFER_SIZE);
            read(new_socket, buffer, BUFFER_SIZE);
            printf("Received TCP message: %s\n", buffer);
            close(new_socket); // Закрываем соединение с TCP-клиентом
        }

        // Проверяем, если на UDP-сокете есть данные
        if (FD_ISSET(udp_socket, &read_fds)) {
            struct sockaddr_in client_addr;
            socklen_t client_addr_len = sizeof(client_addr);
            memset(buffer, 0, BUFFER_SIZE);

            // Чтение данных от UDP-клиента
            int len = recvfrom(udp_socket, buffer, BUFFER_SIZE, 0, (struct sockaddr *)&client_addr, &client_addr_len);
            if (len < 0) {
                perror("recvfrom error");
                close(tcp_socket);
                close(udp_socket);
                exit(EXIT_FAILURE);
            }
            printf("Received UDP message: %s\n", buffer);
        }
    }

    close(tcp_socket);
    close(udp_socket);
    return 0;
}
