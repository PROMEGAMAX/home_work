#include <fcntl.h>
#include <pthread.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>

#define MAX_CLIENT_SIZE 10
#define MAX_MSG_SIZE 256
#define MAX_NAME_SIZE 32
#define SHM_NAME "/chat_shm"

typedef struct {
  char name[MAX_NAME_SIZE];
  char text[MAX_MSG_SIZE];
} Message;

typedef struct {
  int client_count;
  Message messages[100];
} SharedMemory;

SharedMemory *shm_ptr;
pthread_mutex_t shm_mutex = PTHREAD_MUTEX_INITIALIZER;

void sigint_handler(int sig) {
  if (shm_ptr != NULL) {
    munmap(shm_ptr, sizeof(SharedMemory));
    shm_unlink(SHM_NAME);
  }
  printf("\nServer shutting down\n");
  exit(0);
}

void *broadcast_messages(void *arg) {
  while (1) {
    pthread_mutex_lock(&shm_mutex);
    pthread_mutex_unlock(&shm_mutex);
    sleep(1);
  }
  return NULL;
}

int main() {
  int shm_fd = shm_open(SHM_NAME, O_CREAT | O_RDWR, 0666);
  if (shm_fd == -1) {
    perror("shm_open failed");
    exit(1);
  }

  if (ftruncate(shm_fd, sizeof(SharedMemory)) == -1) {
    perror("ftruncate failed");
    exit(1);
  }

  shm_ptr = mmap(NULL, sizeof(SharedMemory), PROT_READ | PROT_WRITE, MAP_SHARED,
                 shm_fd, 0);
  if (shm_ptr == MAP_FAILED) {
    perror("mmap failed");
    exit(1);
  }

  shm_ptr->client_count = 0;

  signal(SIGINT, sigint_handler);
  printf("Server started\n");

  pthread_t broadcast_thread;
  pthread_create(&broadcast_thread, NULL, broadcast_messages, NULL);

  while (1) {
    sleep(1);
  }

  sigint_handler(0);
  return 0;
}
