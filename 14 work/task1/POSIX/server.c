#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <unistd.h>

#define SMEMORY_NAME "/my_shm"
#define  SIZE 1024

int main(){
    int shm_fd;
    char *shm_ptr;

    shm_fd = shm_open(SMEMORY_NAME, O_CREAT | O_RDWR, 0666);
    ftruncate(shm_fd, SIZE);

    shm_ptr = mmap(0, SIZE, PROT_WRITE, MAP_SHARED, shm_fd, 0);

    sprintf(shm_ptr, "Hi!");

    while(strcmp(shm_ptr, "Hello!") != 0){
        sleep(1);
    }

    printf("Client: %s\n",  shm_ptr);

    munmap(shm_ptr, SIZE);
    shm_unlink(SMEMORY_NAME);

    return 0;
}