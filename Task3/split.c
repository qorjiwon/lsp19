#include <stdio.h>
#include <unistd.h>

#define	MAX_BUF	1024

long filesize(FILE *fp) // 파일 크기를 반환하는 함수
{
	long	cur, size;

	cur = ftell(fp); // 현재 파일 포인터 위치 저장
	fseek(fp, 0L, SEEK_END); // 파일 포인터를 파일의 끝으로 이동
	size = ftell(fp); // 파일 크기 저장
	fseek(fp, cur, SEEK_SET); // 파일 포인터를 이전 위치로 복원
	return(size);
}

main(int argc, char *argv[])
{
	FILE	*src, *dst1, *dst2;
	char	buf[MAX_BUF];
	int		count, size;

	if (argc != 4)  {
		fprintf(stderr, "Usage: %s source dest1 dest2\n", argv[0]);
		exit(1);
	}

	if ((src = fopen(argv[1], "rb")) == NULL)  {
		perror("fopen");
		exit(1);
	}
	if ((dst1 = fopen(argv[2], "wb")) == NULL)  {
		perror("fopen");
		exit(1);
	}
	if ((dst2 = fopen(argv[3], "wb")) == NULL)  {
		perror("fopen");
		exit(1);
	}

	size = filesize(src) / 2;

	// 입력 파일의 절반을 첫 번째 파일에 복사
	while (size > 0)  {
		count = (size > MAX_BUF) ?  MAX_BUF : size;
		fread(buf, 1, count, src); 
		fwrite(buf, 1, count, dst1);
		size -= count;
	}

	// 남은 데이터를 두 번째 파일에 복사
	while ((count = fread(buf, 1, MAX_BUF, src)) > 0)  {
		fwrite(buf, 1, count, dst2);
	}

	fclose(src);
	fclose(dst1);
	fclose(dst2);
}
