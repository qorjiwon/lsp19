#include <stdio.h>
#include <unistd.h>

main(int argc, char *argv[])
{
    if (argc != 2)  {
        // 사용법 오류 메시지를 출력하고 프로그램 종료
        fprintf(stderr, "Usage: %s dirname\n", argv[0]);
        exit(1);
    }

    if (chdir(argv[1]) < 0)  {
        // 지정한 디렉토리로 이동하는 데 실패한 경우 에러 메시지를 출력하고 프로그램 종료
        perror("chdir");
        exit(1);
    }
}

