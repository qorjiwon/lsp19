#include <stdio.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdlib.h>
#include <unistd.h>
#include "semlib.c"
#include "prodcons.h"
#include <pthread.h>

char	*ptr, *pData;

int main()
{
	int		shmid;
	int		semid;
	pthread_t       tid1, tid2;

	// 클라이언트의 요청 대기
	semid = semInit(MUTEX_SEM_KEY);
	semInitValue(semid, 0);
	semWait(semid);

	if (pthread_create(&tid1, NULL, (void *)PrintMsg, (void *)msg1) < 0)  {
        perror("pthread_create");
        exit(1);
    }

        if (pthread_create(&tid2, NULL, (void *)PrintMsg, (void *)msg2) < 0)  {
                perror("pthread_create");
                exit(1);
        }
	// 데이터 영역에서 클라이언트의 요청 메시지 출력
	pData = ptr + sizeof(int);
	printf("Server: Received request: %s.....\n", pData);

	// 서버의 응답 메시지 작성
	sprintf(pData, "Server: This is a reply from %d.\n", getpid());

	// 클라이언트에게 응답을 알림
	semPost(semid);
	printf("Replied.\n");

	sleep(1);

	// 공유 메모리 삭제
	if (shmctl(shmid, IPC_RMID, 0) < 0)  {
		perror("shmctl");
		exit(1);
	}

	semDestroy(semid);
	return 0;
}
