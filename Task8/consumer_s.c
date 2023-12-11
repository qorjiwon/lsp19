#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include "semlib.h"
#include "prodcons.h"

int main() {
    BoundedBufferType *pBuf; // 공유 메모리로 연결될 버퍼 포인터
    int shmid, i, data; // 공유 메모리 식별자, 루프 변수, 데이터 변수
    int emptySemid, fullSemid, mutexSemid; // 세마포어 식별자 변수

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

    // 세마포어 초기화
    if ((emptySemid = semInit(EMPTY_SEM_KEY)) < 0)  {
        fprintf(stderr, "semInit failure\n");
        exit(1);
    }
    if ((fullSemid = semInit(FULL_SEM_KEY)) < 0)  {
        fprintf(stderr, "semInit failure\n");
        exit(1);
    }
    if ((mutexSemid = semInit(MUTEX_SEM_KEY)) < 0)  {
        fprintf(stderr, "semInit failure\n");
        exit(1);
    }

    // 세마포어 초기값 설정
    if (semInitValue(emptySemid, MAX_BUF) < 0)  {
        fprintf(stderr, "semInitValue failure\n");
        exit(1);
    }
    if (semInitValue(fullSemid, 0) < 0)  {
        fprintf(stderr, "semInitValue failure\n");
        exit(1);
    }
    if (semInitValue(mutexSemid, 1) < 0)  {
        fprintf(stderr, "semInitValue failure\n");
        exit(1);
    }

    srand(0x9999);

    // 데이터 소비 루프
    for (i = 0; i < NLOOPS; i++)  {
        // 꽉 찬 버퍼 세마포어 대기
        if (semWait(fullSemid) < 0)  {
            fprintf(stderr, "semWait failure\n");
            exit(1);
        }
        // 뮤텍스 세마포어 대기
        if (semWait(mutexSemid) < 0)  {
            fprintf(stderr, "semWait failure\n");
            exit(1);
        }

        printf("Consumer: Consuming an item.....\n");
        data = pBuf->buf[pBuf->out].data;
        pBuf->out = (pBuf->out + 1) % MAX_BUF;
        pBuf->counter--;

        // 뮤텍스 세마포어 신호 전송
        if (semPost(mutexSemid) < 0)  {
            fprintf(stderr, "semPost failure\n");
            exit(1);
        }
        // 빈 버퍼 세마포어 신호 전송
        if (semPost(emptySemid) < 0)  {
            fprintf(stderr, "semPost failure\n");
            exit(1);
        }

        usleep((rand() % 100) * 10000);
    }

    printf("Consumer: Consumed %d items.....\n", i);
    printf("Consumer: %d items in buffer.....\n", pBuf->counter);

    return 0;
}
