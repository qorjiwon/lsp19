#include <stdio.h>

main(int argc, char *argv[])
{
	if (argc != 3)  {
		// 명령행 인수 개수가 3이 아닌 경우 사용법을 출력하고 프로그램 종료
		fprintf(stderr, "Usage: %s old new\n", argv[0]);
		exit(1);
	}

	if (rename(argv[1], argv[2]) < 0)  {
		// argv[1] 파일 이름을 argv[2]로 변경
		perror("rename");
		exit(1);
	}
}
