#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>

main()
{
	struct stat	statbuf;

	// "bar" 파일의 메타데이터 불러오기
	if (stat("bar", &statbuf) < 0)  {
		perror("stat");
		exit(1);
	}

	// "bar" 파일의 그룹 쓰기 권한 제거, S_ISUID 비트를 활성화
	if (chmod("bar", (statbuf.st_mode & ~S_IWGRP) | S_ISUID) < 0)  {
		perror("chmod");
		exit(1);
	}

	// "foo" 파일의 사용자 읽기 및 쓰기 권한을 부여하고, 그룹 및 다른 사용자는 읽기 권한 부여.
	if (chmod("foo", S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH) < 0)  {
		perror("chmod");
		exit(1);
	}
}
