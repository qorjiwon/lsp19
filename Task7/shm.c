#include <stdio.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdlib.h>

#define	ARRAY_SIZE	40000
#define	MALLOC_SIZE	100000
#define	SHM_SIZE	100000
#define	SHM_MODE	(SHM_R | SHM_W)

char	Array[ARRAY_SIZE]; // 전역 배열

int main()
{
	int		shmid;
	char	*ptr, *shmptr;

	// 동적으로 할당된 메모리
	if ((ptr = (char *)malloc(MALLOC_SIZE)) == NULL)  {
		perror("malloc");
		exit(1);
	}

	// 공유 메모리 생성
	if ((shmid = shmget(IPC_PRIVATE, SHM_SIZE, SHM_MODE)) < 0)  {
		perror("shmget");
		exit(1);
	}

	// 공유 메모리를 현재 프로세스의 주소 공간에 첨부
	if ((shmptr = shmat(shmid, 0, 0)) == (void *) -1)  {
		perror("shmat");
		exit(1);
	}

	// 각 메모리 영역의 주소 출력
	printf("Array[] from %p to %p\n", &Array[0], &Array[ARRAY_SIZE]);
	printf("Stack around %p\n", &shmid);
	printf("Malloced from %p to %p\n", ptr, ptr+MALLOC_SIZE);
	printf("Shared memory attached from %p to %p\n", shmptr, shmptr+SHM_SIZE);

	// 공유 메모리 삭제
	if (shmctl(shmid, IPC_RMID, 0) < 0)  {
		perror("shmctl");
		exit(1);
	}
	
	return 0;
}
