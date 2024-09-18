#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define SERVER_PORT 8080

typedef struct Task {
    int client_sock;
    int task_data;
    struct Task* next;
} Task;

typedef struct {
    Task* head;
    Task* tail;
} TaskQueue;

void init_queue(TaskQueue* queue) {
    queue->head = NULL;
    queue->tail = NULL;
}

void enqueue(TaskQueue* queue, int client_sock, int task_data) {
    Task* new_task = (Task*)malloc(sizeof(Task));
    new_task->client_sock = client_sock;
    new_task->task_data = task_data;
    new_task->next = NULL;

    if (queue->tail == NULL) {
        queue->head = new_task;
        queue->tail = new_task;
    } else {
        queue->tail->next = new_task;
        queue->tail = new_task;
    }
}

Task* dequeue(TaskQueue* queue) {
    if (queue->head == NULL) {
        return NULL;
    }
    Task* task = queue->head;
    queue->head = task->next;
    if (queue->head == NULL) {
        queue->tail = NULL;
    }
    return task;
}

int main() {
    TaskQueue queue;
    init_queue(&queue);

    int server_sock, client_sock;
    struct sockaddr_in server_addr, client_addr;
    socklen_t client_addr_len = sizeof(client_addr);
    char buffer[256];
    int task_data;

    server_sock = socket(AF_INET, SOCK_STREAM, 0);
    if (server_sock < 0) {
        perror("Socket error");
        exit(1);
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(SERVER_PORT);
    server_addr.sin_addr.s_addr = INADDR_ANY;

    if (bind(server_sock, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        perror("Bind error");
        close(server_sock);
        exit(1);
    }

    listen(server_sock, 5);
    printf("Server: waiting for connections...\n");

    while (1) {
        client_sock = accept(server_sock, (struct sockaddr*)&client_addr, &client_addr_len);
        if (client_sock < 0) {
            perror("Accept error");
            continue;
        }

        recv(client_sock, buffer, sizeof(buffer), 0);
        task_data = atoi(buffer);
        printf("Server: received task %d\n", task_data);

        enqueue(&queue, client_sock, task_data);

        Task* task = dequeue(&queue);
        if (task != NULL) {
            int result = task->task_data * 2;
            printf("Server: processed task %d, result: %d\n", task->task_data, result);

            sprintf(buffer, "%d", result);
            send(task->client_sock, buffer, strlen(buffer), 0);

            close(task->client_sock);
            free(task);
        }
    }

    close(server_sock);
    return 0;
}
