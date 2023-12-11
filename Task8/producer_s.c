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

    srand(0x8888);

    // 데이터 생산 루프
    for (i = 0; i < NLOOPS; i++)  {
        // 빈 버퍼 세마포어 대기
        if (semWait(emptySemid) < 0)  {
            fprintf(stderr, "semWait failure\n");
            exit(1);
        }
        // 뮤텍스 세마포어 대기
        if (semWait(mutexSemid) < 0)  {
            fprintf(stderr, "semWait failure\n");
            exit(1);
        }

        printf("Producer: Producing an item.....\n");
        data = (rand() % 100) * 10000;
        pBuf->buf[pBuf->in].data = data;
        pBuf->in = (pBuf->in + 1) % MAX_BUF;
        pBuf->counter++;

        // 뮤텍스 세마포어 신호 전송
        if (semPost(mutexSemid) < 0)  {
            fprintf(stderr, "semPost failure\n");
            exit(1);
        }
        // 꽉 찬 버퍼 세마포어 신호 전송
        if (semPost(fullSemid) < 0)  {
            fprintf(stderr, "semPost failure\n");
            exit(1);
        }

        usleep(data);
    }

    printf("Producer: Produced %d items.....\n", i);

    // 프로세스 간 동기화를 위해 잠시 대기
    sleep(2);
    printf("Producer: %d items in buffer.....\n", pBuf->counter);

    // 세마포어 및 공유 메모리 제거
    if (semDestroy(emptySemid) < 0)  {
        fprintf(stderr, "semDestroy failure\n");
    }
    if (semDestroy(fullSemid) < 0)  {
        fprintf(stderr, "semDestroy failure\n");
    }
    if (semDestroy(mutexSemid) < 0)  {
        fprintf(stderr, "semDestroy failure\n");
    }
    if (shmctl(shmid, IPC_RMID, 0) < 0)  {
        perror("shmctl");
        exit(1);
    }

    return 0;
}
