#include <stdio.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include "prodcons.h"
#include "semlib.c"
#include <stdlib.h>
#include <unistd.h>

int main()
{
	int		shmid, semid;
	char	*ptr, *pData;

	// 공유 메모리 가져오기
	if ((shmid = shmget(SHM_KEY, SHM_SIZE, SHM_MODE)) < 0)  {
		perror("shmget");
		exit(1);
	}

	// 공유 메모리에 연결
	if ((ptr = shmat(shmid, 0, 0)) == (void *) -1)  {
		perror("shmat");
		exit(1);
	}

	// 데이터 포인터 설정
	pData = ptr + sizeof(int);

	// 데이터 영역에 요청 메시지 작성
	sprintf(pData, "C: This is a request from %d.\n", getpid());

	printf("C: Sent a request.....\n");

	// 서버의 응답을 기다림
	semid = semInit(MUTEX_SEM_KEY);
	semPost(semid);
	semWait(semid);

	// 서버의 응답을 출력
	printf("C: Received reply: %s\n", pData);
	return 0;
}
