#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

void create_processes1() {
    pid_t pid3, pid4;
    int status3, status4;

    pid3 = fork();
    if (pid3 < 0) {
        perror("Процесс 3 не создался");
        exit(1);
    } else if (pid3 == 0) {
        printf("Процесс 3:\n");
        printf("PID: %d\n", getpid());
        printf("PPID: %d\n", getppid());
        exit(0);
    }

    waitpid(pid3, &status3, 0);
    if (WIFEXITED(status3)) {
        printf("Процесс 3 завершился с кодом %d\n", WEXITSTATUS(status3));
    } else {
        printf("Процесс 3 не завершился\n");
    }

    pid4 = fork();
    if (pid4 < 0) {
        perror("Процесс 4 не создался");
        exit(1);
    } else if (pid4 == 0) {
        printf("Процесс 4:\n");
        printf("PID: %d\n", getpid());
        printf("PPID: %d\n", getppid());
        exit(0);
    }

    waitpid(pid4, &status4, 0);
    if (WIFEXITED(status4)) {
        printf("Процесс 4 завершился с кодом %d\n", WEXITSTATUS(status4));
    } else {
        printf("Процесс 4 не завершился\n");
    }
}

void create_processes2() {
    pid_t pid5;
    int status5;

    pid5 = fork();
    if (pid5 < 0) {
        perror("Процесс 5 не создался");
        exit(1);
    } else if (pid5 == 0) {
        printf("Процесс 5:\n");
        printf("PID: %d\n", getpid());
        printf("PPID: %d\n", getppid());
        exit(0);
    }

    waitpid(pid5, &status5, 0);
    if (WIFEXITED(status5)) {
        printf("Процесс 5 завершился с кодом %d\n", WEXITSTATUS(status5));
    } else {
        printf("Процесс 5 не завершился\n");
    }
}

int main() {
    pid_t pid1, pid2;
    int status1, status2;

    pid1 = fork();
    if (pid1 < 0) {
        perror("Процесс 1 не создался");
        exit(1);
    } else if (pid1 == 0) {
        printf("Процесс 1:\n");
        printf("PID: %d\n", getpid());
        printf("PPID: %d\n", getppid());
        create_processes1();
        exit(0);
    }

    waitpid(pid1, &status1, 0);
    if (WIFEXITED(status1)) {
        printf("Процесс 1 завершился с кодом %d\n", WEXITSTATUS(status1));
    } else {
        printf("Процесс 1 не завершился\n");
    }

    pid2 = fork();
    if (pid2 < 0) {
        perror("Процесс 2 не создался");
        exit(1);
    } else if (pid2 == 0) {
        printf("Процесс 2:\n");
        printf("PID: %d\n", getpid());
        printf("PPID: %d\n", getppid());
        create_processes2();
        exit(0);
    }

    waitpid(pid2, &status2, 0);
    if (WIFEXITED(status2)) {
        printf("Процесс 2 завершился с кодом %d\n", WEXITSTATUS(status2));
    } else {
        printf("Процесс 2 не завершился\n");
    }

    return 0;
}
