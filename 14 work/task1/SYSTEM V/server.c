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

    shm_id = shmget(SHM_KEY, SIZE, IPC_CREAT | 0666);
    shm_ptr = (char *) shmat(shm_id, NULL, 0);

    sprintf(shm_ptr, "Hi!");

    while(strcmp(shm_ptr, "Hello!") != 0){
        sleep(1);
    }

    printf("Client: %s\n", shm_ptr);

    shmdt(shm_ptr);
    shmctl(shm_id, IPC_RMID, NULL);

    return 0;
}