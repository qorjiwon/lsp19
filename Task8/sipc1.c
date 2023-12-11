#include <stdio.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdlib.h>
#include <unistd.h>
#include "semlib.h"

int main()
{
	int		shmid;
	char	*ptr, *pData;
	int		*pInt;
	int		semid;

	// 공유 메모리 가져오기
	semid = semInit(SEM_KEY);
    if (semid == -1) {
        perror("semInit");
        exit(1);
    }

	// 공유 메모리에 연결
	if ((ptr = shmat(shmid, 0, 0)) == (void *) -1)  {
		perror("shmat");
		exit(1);
	}

	// 정수 포인터와 데이터 포인터 설정
	pInt = (int *)ptr;

	// 클라이언트의 요청 대기

	// 데이터 영역에서 클라이언트의 요청 메시지 출력
	pData = ptr + sizeof(int);
	printf("Received request: %s.....", pData);

	// 서버의 응답 메시지 작성
	sprintf(pData, "This is a reply from %d.", getpid());

	// 상태 플래그를 변경하여 클라이언트에게 응답을 알림
	*pInt = 0;
	printf("Replied.\n");

	sleep(1);

	// 공유 메모리 삭제
	if (shmctl(shmid, IPC_RMID, 0) < 0)  {
		perror("shmctl");
		exit(1);
	}
	return 0;
}
