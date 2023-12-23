#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>

#define FIFO_NAME "myfifo"

int main() {
    // Open FIFO for writing
    int fd = open(FIFO_NAME, O_WRONLY);
    if (fd == -1) {
        perror("Error opening FIFO for writing");
        exit(EXIT_FAILURE);
    }

    // Send message
    write(fd, "Hello", sizeof("Hello"));
    printf("FIFO2: Sent a message: Hello\n");

    // Close FIFO
    close(fd);

    // Open FIFO for reading
    fd = open(FIFO_NAME, O_RDONLY);
    if (fd == -1) {
        perror("Error opening FIFO for reading");
        exit(EXIT_FAILURE);
    }

    // Read response message
    char message[100];
    read(fd, message, sizeof(message));
    printf("FIFO2: Received a message: %s\n", message);

    // Close FIFO
    close(fd);

    return 0;
}
