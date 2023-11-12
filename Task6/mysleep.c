#include <stdio.h>
#include <signal.h>

void
SigAlarmHandler(int signo)
{
	/* nothing to do, just return to wake up the pause */
	// 아무 작업 없이 그냥 반환하여 pause를 깨우는 역할
	return;
}

unsigned int
mysleep(unsigned int nsecs) // 커스텀 sleep 함수
{
	/* SIGALRM signal handler: SigAlarmHandler */
	if (signal(SIGALRM, SigAlarmHandler))  {
		// 핸들러 등록에 실패한 경우, 원래 sleep 시간을 반환하고 함수 종료
		return nsecs;
	}

	// nsecs 초 후에 SIGALRM 시그널을 발생시키도록 알람 설정
	alarm(nsecs);/* alarm */

	// 시그널이 도착할 때까지 무한히 대기
	pause();

	// alarm을 0으로 설정하여 이전에 설정한 알람을 해제
	return alarm(0);
}

main()
{
	printf("Wait for 5 seconds...\n");

	mysleep(5);
}
