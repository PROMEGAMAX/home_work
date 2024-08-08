#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

#define NUM_SHOPS 3
#define NUM_BUYERS 3

typedef struct {
    int id;
    int capacity;
    pthread_mutex_t mutex;
} Shop;

typedef struct {
    int id;
    int need;
} Buyer;

Shop shops[NUM_SHOPS];
Buyer buyers[NUM_BUYERS];
int buyers_satisfied = 0;
pthread_mutex_t buyers_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t shop_filled[NUM_SHOPS];
pthread_mutex_t loader_mutex = PTHREAD_MUTEX_INITIALIZER;

void* buyer_function(void* arg) {
    Buyer* buyer = (Buyer*)arg;

    while (1) {
        for (int i = 0; i < NUM_SHOPS; i++) {
            pthread_mutex_lock(&shops[i].mutex);

            while (shops[i].capacity == 0) {
                pthread_mutex_unlock(&shops[i].mutex);
                pthread_mutex_lock(&loader_mutex);
                pthread_cond_wait(&shop_filled[i], &loader_mutex);
                pthread_mutex_unlock(&loader_mutex);
                pthread_mutex_lock(&shops[i].mutex);
            }

            int taken = shops[i].capacity;
            if (taken > buyer->need) {
                taken = buyer->need;
            }

            shops[i].capacity -= taken;
            buyer->need -= taken;

            printf("Покупатель %d зашел в магазин %d, там было %d, мне надо %d\n", 
                   buyer->id, shops[i].id, shops[i].capacity + taken, buyer->need);
            printf("Покупатель %d заснул\n", buyer->id);

            pthread_mutex_unlock(&shops[i].mutex);

            sleep(2);

            if (buyer->need <= 0) {
                printf("Покупатель %d завершил покупки\n", buyer->id);
                pthread_mutex_lock(&buyers_mutex);
                buyers_satisfied++;
                pthread_mutex_unlock(&buyers_mutex);
                return NULL;
            }
        }
    }
}

void* loader_function(void* arg) {
    while (1) {
        pthread_mutex_lock(&buyers_mutex);
        if (buyers_satisfied == NUM_BUYERS) {
            pthread_mutex_unlock(&buyers_mutex);
            printf("Все покупатели завершили покупки. Грузчик завершает работу.\n");
            return NULL;
        }
        pthread_mutex_unlock(&buyers_mutex);

        for (int i = 0; i < NUM_SHOPS; i++) {
            pthread_mutex_lock(&shops[i].mutex);
            if (shops[i].capacity < 5000) {
                shops[i].capacity += 5000;
                printf("Грузчик зашел в магазин %d, положил 5000, теперь там %d\n", 
                       shops[i].id, shops[i].capacity);
                pthread_cond_signal(&shop_filled[i]);
            }
            pthread_mutex_unlock(&shops[i].mutex);
        }

        printf("Грузчик заснул\n");
        sleep(1);
    }
}

int main() {
    pthread_t buyers_threads[NUM_BUYERS];
    pthread_t loader_thread;

    for (int i = 0; i < NUM_SHOPS; i++) {
        shops[i].id = i + 1;
        shops[i].capacity = 0;
        pthread_mutex_init(&shops[i].mutex, NULL);
        pthread_cond_init(&shop_filled[i], NULL);
    }

    for (int i = 0; i < NUM_BUYERS; i++) {
        buyers[i].id = i + 1;
        buyers[i].need = rand() % 100000 + 1;
        pthread_create(&buyers_threads[i], NULL, buyer_function, &buyers[i]);
    }

    pthread_create(&loader_thread, NULL, loader_function, NULL);

    for (int i = 0; i < NUM_BUYERS; i++) {
        pthread_join(buyers_threads[i], NULL);
    }

    pthread_join(loader_thread, NULL);

    for (int i = 0; i < NUM_SHOPS; i++) {
        pthread_mutex_destroy(&shops[i].mutex);
        pthread_cond_destroy(&shop_filled[i]);
    }
    pthread_mutex_destroy(&buyers_mutex);
    pthread_mutex_destroy(&loader_mutex);

    return 0;
}
