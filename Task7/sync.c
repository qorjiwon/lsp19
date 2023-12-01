#include <stdio.h>
#include "synclib.h"

#define	NLOOPS	5


int main()
{
	int		i;
	pid_t	pid;

	TELL_WAIT(); // pipe 생성
	if ((pid = fork()) < 0)  {
		perror("fork");
		exit(1);
	}
	else if (pid > 0)  { // 부모
		for (i = 0 ; i < NLOOPS ; i++)  {
			TELL_CHILD(); // 자식 프로세스에게 알림
			printf("Parent: Tell to child\n");
			printf("Parent: Wait for child to tell\n");
			WAIT_CHILD(); // 자식 프로세스의 응답을 기다림
		}
	}
	else  { // 자식
		for (i = 0 ; i < NLOOPS ; i++)  {
			printf("Child: Wait for parent to tell\n");
			WAIT_PARENT(); // 부모 프로세스의 알림을 기다림
			TELL_PARENT(); // 부모 프로세스에게 알
			printf("Child: Tell to parent\n");
		}
	}
	return 0;
}
