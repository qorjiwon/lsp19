#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include "prodcons.h"

// 생산자 프로세스 메인 함수
int main() {
    BoundedBufferType *pBuf; // 공유 메모리로 연결될 버퍼 포인터
    int shmid, i, data; // 공유 메모리 식별자, 루프 변수, 데이터 변수

    // 공유 메모리 식별자 획득
    if ((shmid = shmget(SHM_KEY, SHM_SIZE, SHM_MODE)) < 0)  {
        perror("shmget");
        exit(1);
    }

    // 공유 메모리에 연결
    if ((pBuf = (BoundedBufferType *)shmat(shmid, 0, 0)) == (void *) -1)  {
        perror("shmat");
        exit(1);
    }

    srand(0x8888);

    // 데이터 생산 루프
    for (i = 0; i < NLOOPS; i++)  {
        // 버퍼가 가득 찼을 때 대기
        if (pBuf->counter == MAX_BUF)  {
            printf("Producer: Buffer full. Waiting.....\n");
            while (pBuf->counter == MAX_BUF)
                ;
        }

        // 데이터 생산
        printf("Producer: Producing an item.....\n");
        data = (rand() % 100) * 10000;
        pBuf->buf[pBuf->in].data = data;
        pBuf->in = (pBuf->in + 1) % MAX_BUF;
        pBuf->counter++;

        usleep(data);
    }

    printf("Producer: Produced %d items.....\n", i);

    // 프로세스 간 동기화를 위해 잠시 대기
    sleep(2);

    printf("Producer: %d items in buffer.....\n", pBuf->counter);

    // 공유 메모리 제거
    if (shmctl(shmid, IPC_RMID, 0) < 0)  {
        perror("shmctl");
        exit(1);
    }

    return 0;
}

