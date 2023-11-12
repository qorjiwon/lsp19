#include <stdio.h>
#include <signal.h>
#include <pthread.h>

#define	THREAD_MAIN
/*
#define	THREAD_1
#define	THREAD_2
*/


void
SigIntHandler(int signo) // SIGINT 시그널에 대한 핸들러 함수
{
	printf("Received a SIGINT signal by thread %d\n", pthread_self());
	printf("Terminate this process\n");

	exit(0);
}

void
Thread1(void *dummy) // SIGINT 신호가 발생하기 전까지 무한 루프
{
#ifdef	THREAD_1
	signal(SIGINT, SigIntHandler);
#endif

	while (1)
		;
}

void
Thread2(void *dummy) // SIGINT 신호가 발생하기 전까지 무한 루프
{
#ifdef	THREAD_2
	signal(SIGINT, SigIntHandler);
#endif

	while (1)
		;
}

main()
{

	pthread_t	tid1, tid2;

	// Thread 1 생성
	if (pthread_create(&tid1, NULL, (void *)Thread1, NULL) < 0)  {
		perror("pthread_create");
		exit(1);
	}

	//Thread 2 생성
	if (pthread_create(&tid2, NULL, (void *)Thread2, NULL) < 0)  {
		perror("pthread_create");
		exit(1);
	}
	printf("Create two threads: tid1=%d, tid2=%d\n", tid1, tid2);
	printf("Main thread: tid=%d\n", pthread_self());

#ifdef	THREAD_MAIN
	signal(SIGINT, SigIntHandler);
#endif

	printf("Press ^C to quit\n");

	// Main Thread가 시그널을 대기하는 무한 루프
	for ( ; ; )
		pause();
}

// Main Thread가 SIGINT를 처리할 것이므로 Received a SIGINT signal by thread %d\n는 Main Thread에서만 출력
