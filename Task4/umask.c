#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>

main()
{
	umask(0); // umask 설정을 0으로 초기화
	if (creat("bar", S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | 
				S_IROTH | S_IWOTH) < 0)  {
		perror("creat"); // creat 함수 호출 실패 시 에러 메시지 출력
		exit(1); // 프로그램을 종료하고 오류 코드 1 반환
	}

	umask(S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH); // umask 변경

	// "foo"라는 파일을 생성하고 사용자에게만 읽기, 쓰기 권한 부여
	if (creat("foo", S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | 
				S_IROTH | S_IWOTH) < 0)  {
		perror("creat");
		exit(1);
	}
}
