#include <stdio.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include "shm.h"
#include <stdlib.h>
#include <unistd.h>

int main()
{
	int		shmid;
	char	*ptr, *pData;
	int		*pInt;

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

	// 정수 포인터와 데이터 포인터 설정
	pInt = (int *)ptr;
	pData = ptr + sizeof(int);

	// 데이터 영역에 요청 메시지 작성
	sprintf(pData, "This is a request from %d.", getpid());

	// 정수 포인터를 통해 상태 플래그 설정하여 서버에게 요청을 알림
	*pInt = 1;
	printf("Sent a request.....");

	// 서버의 응답을 기다림
	while ((*pInt) == 1)
		;

	// 서버의 응답을 출력
	printf("Received reply: %s\n", pData);
	return 0;
}
