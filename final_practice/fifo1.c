#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>

#define FIFO_NAME "myfifo"

int main() {
    // Create FIFO
    mkfifo(FIFO_NAME, 0666);

     // printf("FIFO1: Waiting for a message...\n");

    // Open FIFO for reading
    int fd = open(FIFO_NAME, O_RDONLY);
    if (fd == -1) {
        perror("Error opening FIFO for reading");
        exit(EXIT_FAILURE);
    }

    // Read message from FIFO
    char message[100];
    read(fd, message, sizeof(message));
    printf("FIFO1: Received a message: %s\n", message);

    // Close FIFO
    close(fd);

    // Open FIFO for writing
    fd = open(FIFO_NAME, O_WRONLY);
    if (fd == -1) {
        perror("Error opening FIFO for writing");
        exit(EXIT_FAILURE);
    }

    // Send response message
    write(fd, "Hi", sizeof("Hi"));
    printf("FIFO1: Sent a message: Hi\n");

    // Close FIFO
    close(fd);

    // Remove FIFO
    unlink(FIFO_NAME);

    return 0;
}
