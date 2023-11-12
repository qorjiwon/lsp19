#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>

void CharAtaTime(char *str) {
    char *ptr;
    int c, i;

    setbuf(stdout, NULL); // 출력 버퍼를 비활성화하여 문자열이 즉시 출력되도록 설정

    for (ptr = str; c = *ptr++; ) {
        for (i = 0; i < 999999; i++) ; // 단순한 지연을 생성하여 문자를 한 번씩 출력
        putc(c, stdout); // 한 문자를 표준 출력에 출력
    }
}

int main() { // main 함수의 반환 유형을 int로 변경
    pid_t pid;

    if ((pid = fork()) < 0) { // fork 시스템 호출을 사용하여 자식 프로세스 생성
        perror("fork");
        exit(1); // 오류 시 프로그램 종료
    } else if (pid == 0) { // 자식 프로세스의 경우
        CharAtaTime("output from child\n"); // "output from child" 문자열 출력
    } else { // 부모 프로세스의 경우
        CharAtaTime("output from parent\n"); // "output from parent" 문자열 출력
    }

    return 0; // 프로그램 정상 종료
}

