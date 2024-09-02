#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <unistd.h>

#define SHM_KEY 12345
#define SIZE 1024

int main(){
    int shm_id;
    char *shm_ptr;

    shm_id = shmget(SHM_KEY, SIZE, 0666);
    shm_ptr = (char*) shmat(shm_id, NULL, 0);

    printf("Server: %s\n", shm_ptr);

    sprintf(shm_ptr, "Hello!");

    shmdt(shm_ptr);

    return 0;
}