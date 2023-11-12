#include <stdio.h>

main(int argc, char *argv[])
{
	int		i;

	if (argc == 1)  {
		fprintf(stderr, "Usage: %s filename\n", argv[0]);
		exit(1);
	}

	for (i = 1 ; i < argc ; i++)  {
		// 명령행 인수로 지정된 각 파일을 삭제
		if (remove(argv[i]) < 0)  {
			// remove 함수를 사용하여 파일을 삭제하고, 실패한 경우 에러 메시지 출력
			perror("remove");
			exit(1);
		}
	}
}
