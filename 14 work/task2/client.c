#include <fcntl.h>
#include <mqueue.h>
#include <ncurses.h>
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
char name[MAX_NAME_SIZE];

WINDOW *message_win;
WINDOW *input_win;
WINDOW *info_win;

pthread_mutex_t print_mutex = PTHREAD_MUTEX_INITIALIZER;

void sigint_handler(int sig) {
  endwin();
  printf("\nClient shutting down\n");
  exit(0);
}

void *receive_messages(void *arg) {
  int last_received_msg = 0;
  while (1) {
    pthread_mutex_lock(&print_mutex);

    for (int i = last_received_msg; i < shm_ptr->client_count; ++i) {
      if (strcmp(shm_ptr->messages[i].name, name) != 0) {
        wprintw(message_win, "%s: %s\n", shm_ptr->messages[i].name,
                shm_ptr->messages[i].text);
        wrefresh(message_win);
      }
    }
    last_received_msg = shm_ptr->client_count;

    pthread_mutex_unlock(&print_mutex);
    sleep(1);
  }
  return NULL;
}

void init_windows() {
  int height, width;
  getmaxyx(stdscr, height, width);
  message_win = newwin(height - 3, width - 30, 0, 0);
  scrollok(message_win, TRUE);
  info_win = newwin(height - 3, 30, 0, width - 30);
  input_win = newwin(3, width - 30, height - 3, 0);
  wrefresh(message_win);
  wrefresh(input_win);
  wrefresh(info_win);
}

int main() {
  initscr();
  cbreak();
  noecho();
  keypad(stdscr, TRUE);
  init_windows();
  mvwprintw(input_win, 1, 1, "Enter your name: ");
  wrefresh(input_win);
  wgetnstr(input_win, name, MAX_NAME_SIZE);
  name[strcspn(name, "\n")] = '\0';

  int shm_fd = shm_open(SHM_NAME, O_RDWR, 0666);
  if (shm_fd == -1) {
    perror("shm_open failed");
    sigint_handler(0);
  }

  shm_ptr = mmap(NULL, sizeof(SharedMemory), PROT_READ | PROT_WRITE, MAP_SHARED,
                 shm_fd, 0);
  if (shm_ptr == MAP_FAILED) {
    perror("mmap failed");
    sigint_handler(0);
  }

  pthread_t recv_thread;
  pthread_create(&recv_thread, NULL, receive_messages, NULL);

  signal(SIGINT, sigint_handler);

  while (1) {
    char input[MAX_MSG_SIZE];
    wclear(input_win);
    mvwprintw(input_win, 1, 1, "Message: ");
    wrefresh(input_win);
    wgetnstr(input_win, input, MAX_MSG_SIZE);

    if (strncmp(input, "/quit", 5) == 0) {
      sigint_handler(0);
    } else {
      pthread_mutex_lock(&print_mutex);

      Message msg;
      strncpy(msg.name, name, MAX_NAME_SIZE);
      strncpy(msg.text, input, MAX_MSG_SIZE);

      shm_ptr->messages[shm_ptr->client_count] = msg;
      shm_ptr->client_count++;

      wprintw(message_win, "%s: %s\n", name, input);
      wrefresh(message_win);

      pthread_mutex_unlock(&print_mutex);
    }
  }

  sigint_handler(0);
  return 0;
}
