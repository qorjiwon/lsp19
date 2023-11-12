#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>

main() {
    pid_t pid;
    char *argv[] = { "mycp", "exec.c", "exec2.c", NULL };

    // execl
    if ((pid = fork()) < 0) {
        perror("fork"); // fork 실패 시 에러 출력
        exit(1);
    } else if (pid == 0) {
        // 자식 프로세스
        if (execl("/home/cjs/lsp/file-io/mycp", "mycp", "exec.c", "exec1.c", NULL) < 0) {
            perror("execl"); // execl 실패 시 에러 출력
            exit(1);
        }
    }
    // 부모 프로세스
    if (waitpid(pid, NULL, 0) < 0) {
        perror("waitpid"); // waitpid 실패 시 에러 출력
        exit(1);
    }

    // execv
    if ((pid = fork()) < 0) {
        perror("fork");
        exit(1);
    } else if (pid == 0) {
        // 자식 프로세스
        if (execv("/home/cjs/lsp/file-io/mycp", argv) < 0) {
            perror("execv"); // execv 실패 시 에러 출력
            exit(1);
        }
    }
    // 부모 프로세스
    if (waitpid(pid, NULL, 0) < 0) {
        perror("waitpid");
        exit(1);
    }

    // execlp
    if ((pid = fork()) < 0) {
        perror("fork");
        exit(1);
    } else if (pid == 0) {
        // 자식 프로세스
        if (execlp("env", "env", NULL) < 0) {
            perror("execlp"); // execlp 실패 시 에러 출력
            exit(1);
        }
    }
    // 부모 프로세스
    if (waitpid(pid, NULL, 0) < 0) {
        perror("waitpid");
        exit(1);
    }
}

