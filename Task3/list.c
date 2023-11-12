#include <stdio.h>

#define	MAX_BUF	256

main(int argc, char *argv[])
{
	FILE	*fp;
	char	buf[MAX_BUF];
	int		line; // 행 번호를 추적하기 위한 변수

	if (argc != 2)  { // 실행파일, 소스파일
		printf("Usage: %s filename\n", argv[0]);
		exit(1);
	}

	if ((fp = fopen(argv[1], "rt")) == NULL)  {
		perror("fopen");
		exit(1);
	}

	line = 1; // 행 번호 초기화
	while (fgets(buf, MAX_BUF, fp))  {
		printf("%4d: %s", line++, buf); // 행 번호와 텍스트 출력
	}

	fclose(fp);
}
