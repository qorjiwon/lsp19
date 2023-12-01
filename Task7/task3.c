#include <stdio.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include "synclib.h"

#define NLOOPS 10
#define SIZE sizeof(long)

int update(long *ptr) {
    return ((*ptr)++);
}

int main() {
    int shmid, i, counter;
    pid_t pid;
    long *area;

    // Create a new shared memory segment
    if ((shmid = shmget(IPC_PRIVATE, SIZE, IPC_CREAT | 0666)) < 0) {
        perror("shmget");
        exit(1);
    }

    // Attach the shared memory segment
    if ((area = (long *)shmat(shmid, NULL, 0)) == (long *) -1) {
        perror("shmat");
        exit(1);
    }

    TELL_WAIT();

    if ((pid = fork()) < 0) {
        perror("fork");
        exit(1);
    } else if (pid > 0) {
        // Parent Process
        for (i = 0; i < NLOOPS; i += 2) {
            if ((counter = update(area)) != i) {
                fprintf(stderr, "Counter mismatch\n");
                exit(1);
            }
            printf("Parent: counter=%d\n", counter);
            TELL_CHILD();
            WAIT_CHILD();
        }
    } else {
        // Child Process
        for (i = 1; i < NLOOPS; i += 2) {
            WAIT_PARENT();
            if ((counter = update(area)) != i) {
                fprintf(stderr, "Counter mismatch\n");
                exit(1);
            }
            printf("Child : counter=%d\n", counter);
            TELL_PARENT();
        }
    }

    // Detach the shared memory segment
    if (shmdt(area) == -1) {
        perror("shmdt");
        exit(1);
    }

    // Remove the shared memory segment
    if (shmctl(shmid, IPC_RMID, 0) == -1) {
        perror("shmctl");
        exit(1);
    }

    return 0;
}

