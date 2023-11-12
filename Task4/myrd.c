#include <stdio.h>
#include <unistd.h>

main(int argc, char *argv[])
{
        if (argc != 2)  {
                // 명령행 인수 개수가 2가 아닌 경우 사용법을 출력하고 프로그램 종료
                fprintf(stderr, "Usage: %s dirname\n", argv[0]);
                exit(1);
        }

        if (rmdir(argv[1]) < 0)  {
                // rmdir 함수를 사용하여 지정된 디렉토리를 삭제하고, 실패한 경우 에러 메시지를 출력
                perror("rmdir");
                exit(1);
        }
}
