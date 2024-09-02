#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <unistd.h>

#define SMEMORY_NAME "/my_shm"
#define SIZE 1024

int main(){
    int shm_fd;
    char *shm_ptr;

    shm_fd = shm_open(SMEMORY_NAME, O_RDWR, 0666);
    shm_ptr = mmap(0, SIZE,  PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);

    printf("Server: %s\n", shm_ptr);
    sprintf(shm_ptr, "Hello!");

    munmap(shm_ptr, SIZE);
    
    return 0;
}