#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include "prodcons.h"

// 소비자 프로세스 메인 함수
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

    srand(0x9999);

    // 데이터 소비 루프
    for (i = 0; i < NLOOPS; i++)  {
        // 버퍼가 비어 있을 때 대기
        if (pBuf->counter == 0)  {
            printf("Consumer: Buffer empty. Waiting.....\n");
            while (pBuf->counter == 0)
                ;
        }

        // 데이터 소비
        printf("Consumer: Consuming an item.....\n");
        data = pBuf->buf[pBuf->out].data;
        pBuf->out = (pBuf->out + 1) % MAX_BUF;
        pBuf->counter--;

        usleep((rand() % 100) * 10000);
    }

    printf("Consumer: Consumed %d items.....\n", i);
    printf("Consumer: %d items in buffer.....\n", pBuf->counter);

    return 0;
}
