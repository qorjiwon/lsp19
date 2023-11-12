#include <sys/types.h>
#include <dirent.h>
#include <stddef.h>

main()
{
        DIR             *dp;              // 디렉토리 스트림을 나타내는 포인터
        struct dirent   *dep;            // 디렉토리 엔트리 정보를 담는 구조체 포인터

        if ((dp = opendir(".")) == NULL)  {
                // 현재 디렉토리를 열고, 열기에 실패한 경우 에러 메시지를 출력하고 프로그램 종료
                perror("opendir");
                exit(0);
        }

        while (dep = readdir(dp))  {
                // 디렉토리에서 다음 엔트리를 읽어옴
                printf("%s\n", dep->d_name); // 읽어온 엔트리의 이름을 출력
        }

        closedir(dp); // 디렉토리 스트림을 닫음
}
