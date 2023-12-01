#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>

static int	Pfd1[2], Pfd2[2];

void TELL_WAIT(void) // pipe 생성
{
	if (pipe(Pfd1) < 0 || pipe(Pfd2) < 0)  {
		perror("pipe");
		exit(1);
	}
}

void TELL_PARENT(void) // 부모에게 알리는 함수
{
	if (write(Pfd2[1], "c", 1) != 1)  {
		perror("write");
		exit(1);
	}
}

void WAIT_PARENT(void) // 자식이 부모의 알림을 기다리는 함수
{
	char	c;

	if (read(Pfd1[0], &c, 1) != 1)  {
		perror("read");
		exit(1);
	}
	if (c != 'p')  {
		fprintf(stderr, "WAIT_PARENT: incorrect data");
		exit(1);
	}
}

void TELL_CHILD(void) // 자식에게 알리는 함수
{
	if (write(Pfd1[1], "p", 1) != 1)  {
		perror("write");
		exit(1);
	}
}

void WAIT_CHILD(void) // 자식을 기다리는 함수
{
	char	c;

	if (read(Pfd2[0], &c, 1) != 1)  {
		perror("read");
		exit(1);
	}
	if (c != 'c')  {
		fprintf(stderr, "WAIT_CHILD: incorrect data");
		exit(1);
	}
}
