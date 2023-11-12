#include <stdio.h>
#include <signal.h>
#include <pthread.h>

pthread_t	ThreadId[2];

void
SigIntHandler(int signo)
{
	int		i;

	printf("Received a SIGINT signal by thread %d\n", pthread_self());
	printf("Terminate other threads: tid=%d, %d\n", ThreadId[0], ThreadId[1]);

	for (i = 0 ; i < 2 ; i++)  { // 스레드들 종료
		/* Cancel execution of all threads */
		if (pthread_cancel(ThreadId[i]))  {
			perror("pthread_cancel");
			exit(1);
		}
	}
	for (i = 0 ; i < 2 ; i++)  { // 종료된 스레드들의 진짜 종료(자원 정리)를  기다림
		if (pthread_join(ThreadId[i], NULL))  {
			perror("pthread_join");
			exit(1);
		}
	}
	printf("Threads terminated: tid=%d, %d\n", ThreadId[0], ThreadId[1]);

	exit(0);
}

void
Thread1(void *dummy)
{
	// 스레드 취소 가능 및 취소 타입 설정
	printf("Thread %d created.....\n", pthread_self());

	if (pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL))  {
		perror("pthread_setcancelstate");
		pthread_exit(NULL);
	}
	if (pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS, NULL))  {
		perror("pthread_setcanceltype");
		pthread_exit(NULL);
	}

	while (1)
		;
}

void
Thread2(void *dummy)
{
	printf("Thread %d created.....\n", pthread_self());

	// 스레드 취소 가능. 취소 타입 저장하지 않음
	if (pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL))  {
		perror("pthread_setcancelstate");
		pthread_exit(NULL);
	}
	if (pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS, NULL))  {
		perror("pthread_setcanceltype");
		pthread_exit(NULL);
	}

	while (1)
		;
}



main()
{
	// Thread1, 2 생성
	if (pthread_create(&ThreadId[0], NULL, (void *)Thread1, NULL) < 0)  {
		perror("pthread_create");
		exit(1);
	}
	if (pthread_create(&ThreadId[1], NULL, (void *)Thread2, NULL) < 0)  {
		perror("pthread_create");
		exit(1);
	}

	signal(SIGINT, SigIntHandler); // SIGINT 시그널 처리

	printf("Press ^C to quit\n");

	for ( ; ; ) // 무한 루프. 평범하게 종료되지 않음.
		pause();
}

// 용자가 Ctrl+C를 입력하면 SigIntHandler 함수가 호출되어 다른 스레드들을 종료하고, 프로그램이 정상적으로 종료된다.
