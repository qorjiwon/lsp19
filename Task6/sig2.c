#include <stdio.h>
#include <signal.h>

void
SigUsrHandler(int signo)
{
	if (signo == SIGUSR1)  {
		printf("Received a SIGUSR1 signal\n");
	}
	else if (signo == SIGUSR2)  {
		printf("Received a SIGUSR2 signal\n");
	}
	else  {
		printf("Received unknown signal\n");
		printf("Terminate this process\n");
		exit(0);
	}
}

main()
{
	if (signal(SIGUSR1, SigUsrHandler) == SIG_ERR)  {
		// SIGUSR1 시그널에 대한 핸들러 등록이 실패한 경우 실행
		perror("signal");
		exit(1);
	}
	
	if (signal(SIGUSR2, SigUsrHandler) == SIG_ERR)  {
		//SIGUSR2 시그널에 대한 핸들러 등록이 실패한 경우 실행
		perror("signal");
		exit(1);
	}

	for ( ; ; )
		pause();
}

// make sig2
// ./sig2 (커맨드 창1)
// ps -u lsp19 (커맨드 창2)
// sig2의 PID이 1068이라고 가정
// kill -USR1 1068 (커맨드 창2)
// -> Received a SIGUSR1 signal 출력 확인 (커맨드 창1)
// kill -USR2 1068 (커맨드 창2)
// -> Received a SIGUSR1 signal 출력 확인(커맨드 창1)
// kull -TERM 1068 (커맨드 창2_
// -> [1]    1068 terminated  ./sig2 출력 확인 (커맨드 창1)
