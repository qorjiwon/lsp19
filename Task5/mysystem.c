#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

int mysystem(const char *cmd) {
    pid_t child_pid;
    int status;

    // fork a child process
    child_pid = fork();

    if (child_pid == -1) {
        perror("fork");
        return -1;
    }

    if (child_pid == 0) {
        // Child process
        const char *shell = "/bin/sh"; // Use the default shell
        const char *shell_flag = "-c"; // -c flag to execute the command
        char *cmd_string = strdup(cmd); // Duplicate the command

        if (cmd_string == NULL) {
            perror("strdup");
            exit(1);
        }

        // Execute the command using /bin/sh
        char *const argv[] = {(char *)shell, (char *)shell_flag, cmd_string, NULL};
        execv(shell, argv);

        // If execv fails, exit the child process
        perror("execv");
        exit(1);
    } else {
        // Parent process
        // Wait for the child process to finish
        if (waitpid(child_pid, &status, 0) == -1) {
            perror("waitpid");
            return -1;
        }

        if (WIFEXITED(status)) {
            // Child process terminated normally, return its exit status
            return WEXITSTATUS(status);
        } else {
            // Child process didn't terminate normally
            return -1;
        }
    }
}

int main() {
    int exit_status;

    // Run a command using mysystem
    printf("Running mysystem(\"ls -l\")\n");
    exit_status = mysystem("ls -l");

    if (exit_status == -1) {
        printf("Failed to run the command.\n");
    } else {
        printf("Command exited with status %d\n", exit_status);
    }

    return 0;
}
