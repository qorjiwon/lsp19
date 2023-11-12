#include <stdio.h>
#include <signal.h>
#include <pwd.h>

// SIGALRM 시그널에 대한 핸들러 함수
void
MyAlarmHandler(int signo)
{
	struct passwd	*rootptr;

	signal(SIGALRM, MyAlarmHandler);
	alarm(1); // 1초 주기로 알람 설정

	printf("in signal handler\n");
	
	// getpe
	if ((rootptr = getpwnam("root")) == NULL)  {
		perror("getpwnam");
		exit(1);
	}

	return;
}

main()
{
	struct passwd	*ptr;

	signal(SIGALRM, MyAlarmHandler); // 시그널 핸들러 등록
	alarm(1); // 1초 주기로 알람 설정

	for ( ; ; )  {
		// getpwnam 함수를 사용하여 "lsp19" 사용자 정보 확인
		if ((ptr = getpwnam("lsp19")) == NULL)  {
			perror("getpwnam");
			exit(1);
		}

		// 사용자 정보가 손상되었는지 확인하고, 손상되었으면프로그램 종료
		if (strcmp(ptr->pw_name, "lsp19") != 0)  {
			printf("return value corrupted!, pw_name = %s\n", ptr->pw_name);
			exit(0);
		}
	}
}
