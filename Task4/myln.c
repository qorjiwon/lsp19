#include <stdio.h>
#include <unistd.h>

main(int argc, char *argv[])
{
	if (argc != 3)  {
		// 명령행 인수 개수가 3이 아닌 경우 사용법을 출력하고 프로그램 종료
		fprintf(stderr, "Usage: %s source destinaion\n", argv[0]);
		exit(1);
	}

	if (link(argv[1], argv[2]) < 0)  {
		// // link 함수를 사용하여 하드 링크를 생성하고, 실패한 경우 에러 메시지를 출력
		perror("link");
		exit(1);
	}
}
