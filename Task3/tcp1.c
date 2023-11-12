#include <stdio.h>

main(int argc, char *argv[])
{
	FILE	*src, *dst;
	int		ch;

	if (argc != 3)  { // 실행 파일, 소스 파일, 대상 파일
		fprintf(stderr, "Usage: %s source destination\n", argv[0]);
		exit(1);
	}

	if ((src = fopen(argv[1], "rt")) == NULL)  {
		perror("fopen");
		exit(1);
	}

	if ((dst = fopen(argv[2], "wt")) == NULL)  {
		perror("fopen");
		exit(1);
	}

	// src에서 한 문자씩 읽어서 dst에 저장
	while ((ch = fgetc(src)) != EOF)  {
		fputc(ch, dst);
	}

	fclose(src);
	fclose(dst);
}
