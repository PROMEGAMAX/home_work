#include <pthread.h>
#include <stdio.h>

int counter = 0;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

void *incrementCounter(void *arg) {
  for (int i = 0; i < 100000; i++) {
    pthread_mutex_lock(&mutex);
    counter++;
    pthread_mutex_unlock(&mutex);
  }
  return NULL;
}

int main() {
  pthread_t thread1, thread2;

  pthread_create(&thread1, NULL, incrementCounter, NULL);
  pthread_create(&thread2, NULL, incrementCounter, NULL);

  pthread_join(thread1, NULL);
  pthread_join(thread2, NULL);

  printf("Counter: %d\n", counter);
  return 0;
}