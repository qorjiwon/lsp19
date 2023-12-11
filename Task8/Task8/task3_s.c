#include <stdio.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdlib.h>
#include <unistd.h>
#include "semlib.c"
#include "prodcons.h"
#include <pthread.h>
#define MAX 100
int		semid;
char	data[MAX];

void Server(){
	semInitValue(semid, 0);
	semWait(semid);

	// 데이터 영역에서 클라이언트의 요청 메시지 출력
	printf("Server: Received request: %s.....\n", data);

	// 서버의 응답 메시지 작성
	sprintf(data, "Server: This is a reply from %d.\n", getpid());

	// 클라이언트에게 응답을 알림
	semPost(semid);
	printf("Replied.\n");
}

void Client(){
	
	// 데이터 영역에 요청 메시지 작성
	sprintf(data, "C: This is a request from %d.\n", getpid());
	printf("C: Sent a request.....\n");

	// 서버의 응답을 기다림
	semPost(semid);
	semWait(semid);

	// 서버의 응답을 출력
	printf("C: Received reply: %s\n", data);
}

int main()
{
	pthread_t       tid1, tid2;
	semid = semInit(MUTEX_SEM_KEY);

	if (pthread_create(&tid1, NULL, (void *)Server, NULL) < 0)  {
        perror("pthread_create");
        exit(1);
    }

	if (pthread_create(&tid2, NULL, (void *)Client, NULL) < 0)  {
			perror("pthread_create");
			exit(1);
	}

	if (pthread_join(tid1, NULL) < 0) {
        perror("pthread_join");
        exit(1);
    }
    if (pthread_join(tid2, NULL) < 0) {
        perror("pthread_join");
        exit(1);
    }

	semDestroy(semid);
	return 0;
}
