#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>

main()
{
	int 	fd;
	char	ch = '\0';

	if ((fd = creat("file.hole", 0400)) < 0)  {
		perror("creat");
		exit(1);
	}

	if (lseek(fd, 1023, SEEK_SET) < 0)  {
		perror("lseek"); // 파일 오프셋 이동 시 오류 발생하면 에러 메시지 출력
		exit(1);
	}
	write(fd, &ch, 1); // 파일에 ch('\0') 쓰기

	close(fd);
}
