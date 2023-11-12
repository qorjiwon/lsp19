#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>

main(int argc, char *argv[])
{
        if (argc != 2)  {
                // 명령행 인수 개수가 2가 아닌 경우 사용법을 출력하고 프로그램 종료
                fprintf(stderr, "Usage: %s dirname\n", argv[0]);
                exit(1);
        }

        if (mkdir(argv[1], 0755) < 0)  {
                // mkdir 함수를 사용하여 디렉토리를 생성하고, 실패한 경우 에러 메시지를 출력
                perror("mkdir");
                exit(1);
        }
}
