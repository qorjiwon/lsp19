#include <stdio.h>
#include <stdlib.h>

void myexit1() { // 첫 번째 종료 핸들러 함수
    printf("first exit handler\n");
}

void myexit2() { // 두 번째 종료 핸들러 함수
    printf("second exit handler\n");
}

int main() { // main 함수의 반환 유형을 int로 변경
    if (atexit(myexit2) != 0) { // atexit 함수를 사용하여 두 번째 종료 핸들러 등록
        perror("atexit");
        exit(1); // 오류 시 프로그램 종료
    }

    if (atexit(myexit1) != 0) { // atexit 함수를 사용하여 첫 번째 종료 핸들러 등록
        perror("atexit");
        exit(1); // 오류 시 프로그램 종료
    }

    if (atexit(myexit1) != 0) { // atexit 함수를 사용하여 첫 번째 종료 핸들러를 다시 등록
        perror("atexit");
        exit(1); // 오류 시 프로그램 종료
    }

    printf("main is done\n"); // "main is done" 출력
    return 0; // 프로그램 정상 종료
}

