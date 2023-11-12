#include <stdio.h>
#include <sys/types.h>
#include <dirent.h>
#include <sys/stat.h>

#define MAX_PATH 256

// 디렉토리 트리를 순회하며 디렉토리 내용을 출력하는 재귀 함수
void JustDoIt(char *path)
{
    DIR             *dp;                // 디렉토리 스트림을 나타내는 포인터
    struct dirent   *dep;              // 디렉토리 엔트리 정보를 담는 구조체 포인터
    struct stat     statbuf;           // 파일/디렉토리의 메타데이터를 저장하는 구조체
    char            fullpath[MAX_PATH]; // 전체 파일 경로를 저장하기 위한 배열

    if ((dp = opendir(path)) == NULL)  {
        // 디렉토리를 열고, 열기에 실패한 경우 에러 메시지를 출력하고 프로그램 종료
        perror("opendir");
        exit(0);
    }

    printf("\n%s:\n", path); // 현재 디렉토리 이름 출력

    while (dep = readdir(dp))  {
        if (strcmp(".", dep->d_name) == 0 || strcmp("..", dep->d_name) == 0)
            continue;
        printf("%s\n", dep->d_name); // 디렉토리 내의 파일/디렉토리 이름 출력
    }

    rewinddir(dp); // 디렉토리 스트림을 처음으로 되감음

    while (dep = readdir(dp))  {
        if (strcmp(".", dep->d_name) == 0 || strcmp("..", dep->d_name) == 0)
            continue;
        strcpy(fullpath, path);          // 디렉토리 경로를 복사
        strcat(fullpath, "/");           // 슬래시 추가
        strcat(fullpath, dep->d_name);   // 파일/디렉토리 이름 추가
        if (lstat(fullpath, &statbuf) < 0)  {
            // 파일/디렉토리 메타데이터를 읽고, 읽기에 실패한 경우 에러 메시지를 출력하고 프로그램 종료
            perror("lstat");
            exit(1);
        }
        if (S_ISDIR(statbuf.st_mode))  {
            // 현재 엔트리가 디렉토리인 경우 재귀적으로 해당 디렉토리를 처리
            JustDoIt(fullpath);
        }
    }

    closedir(dp); // 디렉토리 스트림을 닫음
}

main()
{
    JustDoIt("."); // 현재 디렉토리부터 시작하여 디렉토리 트리를 순회
}

