#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>

int main() { // main 함수의 반환 유형을 int로 변경
    pid_t pid; // 프로세스 ID를 저장할 변수 pid 선언
    int status; // 자식 프로세스의 종료 상태를 저장할 변수 status 선언

    if ((pid = fork()) < 0) { // fork 시스템 호출을 사용하여 자식 프로세스 생성
        perror("fork");
        exit(1); // 오류 시 프로그램 종료
    } else if (pid == 0) { // 자식 프로세스의 경우
        printf("I'm a child\n"); // "I'm a child" 출력
        sleep(2); // 자식 프로세스를 2초 동안 일시 중지
    } else { // 부모 프로세스의 경우
        wait(&status); // 자식 프로세스의 종료 상태를 기다림
        printf("A child killed with %d status\n", status); // 자식 프로세스의 종료 상태 출력
    }

    return 0; // 프로그램 정상 종료
}

