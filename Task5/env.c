#include <stdio.h>

main(int argc, char *argv[], char *envp[]) {
    int i;
    char **p;
    extern char **environ; // 환경 변수를 저장하는 전역 변수 extern으로 선언

    printf("List command-line arguments\n");
    for (i = 0; i < argc; i++) {
        printf("%s\n", argv[i]); // 명령줄 인수 출력
    }

    printf("\n");
    printf("List environment variables from environ variable\n");
    for (p = environ; *p != NULL; p++) {
        printf("%s\n", *p); // environ 변수를 통해 환경 변수 출력
    }

    printf("\n");
    printf("List environment variables from envp variable\n");
    for (p = envp; *p != NULL; p++) {
        printf("%s\n", *p); // envp 배열을 통해 환경 변수 출력
    }
}:
