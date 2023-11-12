#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <dirent.h>
#include <sys/stat.h>
#include <pwd.h>
#include <grp.h>
#include <time.h>

int main(int argc, char *argv[]) {
    // 사용자가 입력한 디렉토리 경로를 받거나, 기본적으로 현재 디렉토리를 사용합니다.
    char *dir_path = (argc > 1) ? argv[1] : ".";

    DIR *dir;
    struct dirent *entry;
    struct stat file_info;
    long long total_blocks = 0; // 블록 수를 누적할 변수

    // 디렉토리 열기
    if ((dir = opendir(dir_path)) == NULL) {
        perror("opendir"); // 디렉토리 열기 실패 시 에러 메시지 출력
        exit(1);
    }

    while ((entry = readdir(dir)) != NULL) {
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)
		continue; // 현재 디렉토리 및 상위 디렉토리는 출력하지 않음
	char full_path[PATH_MAX];
        // 파일의 전체 경로 생성
        snprintf(full_path, sizeof(full_path), "%s/%s", dir_path, entry->d_name);

        if (lstat(full_path, &file_info) < 0) {
            perror("lstat"); // 파일 정보 읽기 실패 시 에러 메시지 출력
            exit(1);
        }

        struct passwd *user_info = getpwuid(file_info.st_uid);
        struct group *group_info = getgrgid(file_info.st_gid);

        char permission[11];
        // 파일 권한 문자열 생성
        snprintf(permission, sizeof(permission), "%c%c%c%c%c%c%c%c%c%c",
                 (S_ISDIR(file_info.st_mode)) ? 'd' : '-', // 디렉토리 여부 확인
                 (file_info.st_mode & S_IRUSR) ? 'r' : '-',
                 (file_info.st_mode & S_IWUSR) ? 'w' : '-',
                 (file_info.st_mode & S_IXUSR) ? 'x' : '-',
                 (file_info.st_mode & S_IRGRP) ? 'r' : '-',
                 (file_info.st_mode & S_IWGRP) ? 'w' : '-',
                 (file_info.st_mode & S_IXGRP) ? 'x' : '-',
                 (file_info.st_mode & S_IROTH) ? 'r' : '-',
                 (file_info.st_mode & S_IWOTH) ? 'w' : '-',
                 (file_info.st_mode & S_IXOTH) ? 'x' : '-');

	total_blocks += file_info.st_blocks; // 전체 블록 수 누적
        char time_buffer[80];
        struct tm *timeinfo = localtime(&file_info.st_mtime);
        // 파일 수정 시간을 포맷팅
        strftime(time_buffer, sizeof(time_buffer), "%b %d %H:%M", timeinfo);

        // 파일 정보 출력
        printf("%s %2ld %s %s %6ld %s %s\n",
               permission, file_info.st_nlink, user_info->pw_name, group_info->gr_name,
               file_info.st_size, time_buffer, entry->d_name);
    }

    printf("total %lld\n", total_blocks / 2); // 1 블록 = 512 bytes
    // 디렉토리 닫기
    closedir(dir);

    return 0;
}
