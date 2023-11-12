#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>

#define	MAX_BUF	1024

main(int argc, char *argv[])
{
	int 	fd1, fd2, count;
	char	buf[MAX_BUF];

	if (argc != 3)  { // 실행 파일, 소스 파일, 대상 파일
		printf("Usage: %s source destination\n", argv[0]);
		exit(1);
	}

	if ((fd1 = open(argv[1], O_RDONLY)) < 0)  {
		perror("open");
		exit(1);
	}

	if ((fd2 = open(argv[2], O_WRONLY | O_CREAT | O_TRUNC, 0644)) < 0)  {
		perror("open");
		exit(1);
	}

	// fd1에서 데이터를 읽고 fd2에 쓰기
	while (count = read(fd1, buf, MAX_BUF))  {
		write(fd2, buf, count);
	}

	close(fd1);
	close(fd2);
}
