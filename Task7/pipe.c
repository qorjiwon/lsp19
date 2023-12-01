#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>

#define	MAX_BUF	128

int main()
{
	int		n, fd[2];
	pid_t	pid;
	char	buf[MAX_BUF];
	
	if (pipe(fd) < 0)  {
		perror("pipe");
		exit(1);
	}

	if ((pid = fork()) < 0)  {
		perror("fork");
		exit(1);
	}
	else if (pid == 0)  {
		close(fd[1]);
		printf("Child : Wait for parent to send data\n");
		if ((n = read(fd[0],buf, MAX_BUF)) < 0)  {
			perror("read");
			exit(1);
		}
		printf("Child : Received data from parent: ");
		fflush(stdout);
		write(STDOUT_FILENO, buf, n);
	}
	else  {
		close(fd[0]);
		// 데이터를 버퍼에 쓰기
		strcpy(buf, "Hello, World!\n");
		printf("Parent: Send data to child\n");
		// 자식 프로세스로 데이터 쓰기
		write(fd[1], buf, strlen(buf)+1);
	}

	exit(0);
}
