#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>

main(int argc, char *argv[])
{
	struct stat	statbuf; // 파일 또는 디렉토리의 메타데이터를 저장하는 구조체
	char		*mode; // 파일 또는 디렉토리 유형을 저장하는 문자열
	int			i;

	for (i = 1 ; i < argc ; i++)  {
		printf("%s: ", argv[i]); // 주어진 파일 또는 디렉토리 이름 출력

		// lstat를 사용하여 파일 또는 디렉토리에 대한 메타데이터를 가져온다.
		if (lstat(argv[i], &statbuf) < 0)  {
			perror("lstat"); // lstat 함수 호출에 실패한 경우 에러 메시지 출력
			continue; // 다음 파일 또는 디렉토리를 처리하기 위해 루프 계속
		}
		// 파일 또는 디렉토리 유형을 확인하고 해당 유형을 mode에 문자열로 저장.
		if (S_ISREG(statbuf.st_mode))
			mode = "regular";
		else if (S_ISDIR(statbuf.st_mode))
			mode = "directory";
		else if (S_ISCHR(statbuf.st_mode))
			mode = "character special";
		else if (S_ISBLK(statbuf.st_mode))
			mode = "block special";
		else if (S_ISFIFO(statbuf.st_mode))
			mode = "FIFO";
		else if (S_ISLNK(statbuf.st_mode))
			mode = "symbolic link";
		else if (S_ISSOCK(statbuf.st_mode))
			mode = "socket";

		// 파일 및 디렉토리 유형 출력
		printf("%s\n", mode);
		
		// 파일 및 디렉토리의 메타데이터 정보 출력
		printf("\tst_mode = %d\n", statbuf.st_mode);
		printf("\tst_ino = %d\n", statbuf.st_ino);
		printf("\tst_dev = %d\n", statbuf.st_dev);
		printf("\tst_rdev = %d\n", statbuf.st_rdev);
		printf("\tst_nlink = %d\n", statbuf.st_nlink);
		printf("\tst_uid = %d\n", statbuf.st_uid);
		printf("\tst_gid = %d\n", statbuf.st_gid);
		printf("\tst_size = %d\n", statbuf.st_size);
		printf("\tst_atime = %d\n", statbuf.st_atime);
		printf("\tst_mtime = %d\n", statbuf.st_mtime);
		printf("\tst_ctime = %d\n", statbuf.st_ctime);
		printf("\tst_blksize = %d\n", statbuf.st_blksize);
		printf("\tst_blocks = %d\n", statbuf.st_blocks);
	}
}
