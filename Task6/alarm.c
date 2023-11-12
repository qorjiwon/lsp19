#include <stdio.h>
#include <signal.h>

// AlarmSecs: 주기적 알람의 주기를 저장하는 전역 변수
static unsigned int	AlarmSecs;

// SIGALRM 시그널에 대한 핸들러 함수
void
SigAlarmHandler(int signo)
{
	if (signal(SIGALRM, SigAlarmHandler) == SIG_ERR)  {
		perror("signal");
		exit(1);
	}

	// 주기적으로 알람 설정
	alarm(AlarmSecs);/* alarm */

	// 주기적으로 수행할 작업
	/* Do something */
	printf("."); // \n이 없으므로 계속 버퍼에 저장
	fflush(stdout); // 화면에 출력

	return;
}

//주기적으로 알람을 설정하는 함수
int
SetPeriodicAlarm(unsigned int nsecs)
{
	// SIGALRM 시그널에 대한 핸들러 등록
	if (signal(SIGALRM,SigAlarmHandler) == SIG_ERR)  {
		return -1;
	}

	// AlarmSecs 변수에 주기 저장
	AlarmSecs = nsecs;

	// nsecs 초 주기로 알람 설정
	alarm(nsecs);

	return 0;
}

main()
{
	printf("Doing something every one seconds\n");

	// 1초 주기로 주기적 알람 설정
	SetPeriodicAlarm(1);

	// 무한 루프에서 시그널을 대기
	for ( ; ; )
		pause();
}
