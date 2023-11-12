nclude <stdio.h>
#include <sys/types.h>
#include <unistd.h>

int main() { // main 함수의 반환 유형을 int로 변경
    int pid;

    if ((pid = fork()) < 0) { // fork 시스템 호출을 사용하여 자식 프로세스를 생성
        perror("fork");
        exit(1); // 오류 시 프로그램 종료
    } else if (pid == 0) { // 자식 프로세스의 경우
        /* child */
        printf("I am %d. My parent is %d.\n", getpid(), getppid()); // 자식 프로세스의 프로세스 ID 및 부모 프로세스의 프로세스 ID 출력
    } else { // 부모 프로세스의 경우
        /* parent */
        printf("I am %d. My child is %d.\n", getpid(), pid); // 부모 프로세스의 프로세스 ID 및 자식 프로세스의 프로세스 ID 출력
    }
    return 0; // 프로그램 정상 종료
}
