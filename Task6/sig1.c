#include <stdio.h>
#include <signal.h>

void
SigIntHandler(int signo)
{
	printf("Received a SIGINT signal\n"); // Ctrl+C 입력 시 출력할 메시지
	printf("Terminate this process\n");

	exit(0); // 프로그램 종료
}

main()
{
	signal(SIGINT, SigIntHandler); /* SIGINT signal handler: SigIntHandler */
	/* signal */

	printf("Press ^C to quit\n");

	for ( ; ; ) // signal이 발생할 때까지 대기
		pause();
}

// make sig1
// ./sig1
// Ctrl+C 시 종료
