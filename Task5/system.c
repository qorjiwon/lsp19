#include <stdio.h>
#include <stdlib.h>

main() {
    int status;

    // "date" 명령어를 실행하여 현재 날짜와 시간을 표시하고, 종료 상태를 받아옵니다.
    if ((status = system("date")) < 0) {
        perror("system"); // 시스템 함수 실패 시 에러 출력
        exit(1);
    }
    printf("exit status = %d\n", status);

    // "nosuchcommand" 명령어는 존재하지 않는 명령어이므로 시스템 함수는 에러를 반환할 것입니다.
    if ((status = system("nosuchcommand")) < 0) {
        perror("system");
        exit(1);
    }
    printf("exit status = %d\n", status);

    // "who; exit 44" 명령어는 "who" 명령을 실행하고, 그 결과와 함께 상태 코드 44로 종료합니다.
    if ((status = system("who; exit 44")) < 0) {
        perror("system");
        exit(1);
    }
    printf("exit status = %d\n", status);
}
