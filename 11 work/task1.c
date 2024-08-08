#include <pthread.h>
#include <stdio.h>

void *thread_func(void *arg) {
  int index = *(int *)arg;
  printf("Поток %d запущен\n", index);
  return NULL;
}

int main() {
  int indexes[5];
  for (int i = 0; i < 5; i++) {
    indexes[i] = i;
  }

  pthread_t threads[5];
  for (int i = 0; i < 5; i++) {
    pthread_create(&threads[i], NULL, thread_func, &indexes[i]);
    sleep(1);
  }

  for (int i = 0; i < 5; i++) {
    pthread_join(threads[i], NULL);
  }
  return 0;
}