#include <stdio.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdlib.h>
#include <unistd.h>
#include "semlib.c"
#include "prodcons.h"
#include <pthread.h>

BoundedBufferType Buf;
pthread_cond_t NotFull;
pthread_cond_t NotEmpty;
pthread_mutex_t Mutex;

void ThreadUsleep(int usecs) {
    pthread_cond_t cond;
    pthread_mutex_t mutex;
    struct timespec ts;
    struct timeval tv;

    // 조건 변수와 뮤텍스 초기화
    if (pthread_cond_init(&cond, NULL) < 0) {
        perror("pthread_cond_init");
        pthread_exit(NULL);
    }
    if (pthread_mutex_init(&mutex, NULL) < 0) {
        perror("pthread_mutex_init");
        pthread_exit(NULL);
    }

    gettimeofday(&tv, NULL);
    ts.tv_sec = tv.tv_sec + usecs / 1000000;
    ts.tv_nsec = (tv.tv_usec + (usecs % 1000000)) * 1000;
    if (ts.tv_nsec >= 1000000000) {
        ts.tv_nsec -= 1000000000;
        ts.tv_sec++;
    }

    // 뮤텍스 락 후 조건 변수 대기
    if (pthread_mutex_lock(&mutex) < 0) {
        perror("pthread_mutex_lock");
        pthread_exit(NULL);
    }
    if (pthread_cond_timedwait(&cond, &mutex, &ts) < 0) {
        perror("pthread_cond_timedwait");
        pthread_exit(NULL);
    }

    // 조건 변수 및 뮤텍스 해제
    if (pthread_cond_destroy(&cond) < 0) {
        perror("pthread_cond_destroy");
        pthread_exit(NULL);
    }
    if (pthread_mutex_destroy(&mutex) < 0) {
        perror("pthread_mutex_destroy");
        pthread_exit(NULL);
    }
}

void Producer(void *dummy) {
    int i, data;

    printf("Producer: Start.....\n");

    for (i = 0; i < NLOOPS; i++) {
        if (pthread_mutex_lock(&Mutex) < 0) {
            perror("pthread_mutex_lock");
            pthread_exit(NULL);
        }

        // 버퍼가 꽉 찼을 때 대기
        while (Buf.counter == MAX_BUF) {
            if (pthread_cond_wait(&NotFull, &Mutex) < 0) {
                perror("pthread_cond_wait");
                pthread_exit(NULL);
            }
        }

        printf("Producer: Producing an item.....\n");
        data = (rand() % 100) * 10000;
        Buf.buf[Buf.in].data = data;
        Buf.in = (Buf.in + 1) % MAX_BUF;
        Buf.counter++;

        // 소비자에게 버퍼에 데이터가 있다고 신호
        if (pthread_cond_signal(&NotEmpty) < 0) {
            perror("pthread_cond_signal");
            pthread_exit(NULL);
        }
        if (pthread_mutex_unlock(&Mutex) < 0) {
            perror("pthread_mutex_unlock");
            pthread_exit(NULL);
        }

        ThreadUsleep(data);
    }

    printf("Producer: Produced %d items.....\n", i);
    printf("Producer: %d items in buffer.....\n", Buf.counter);

    pthread_exit(NULL);
}

void Consumer(void *dummy) {
    int i, data;

    printf("Consumer: Start.....\n");

    for (i = 0; i < NLOOPS; i++) {
        if (pthread_mutex_lock(&Mutex) < 0) {
            perror("pthread_mutex_lock");
            pthread_exit(NULL);
        }

        // 버퍼가 비었을 때 대기
        while (Buf.counter == 0) {
            if (pthread_cond_wait(&NotEmpty, &Mutex) < 0) {
                perror("pthread_cond_wait");
                pthread_exit(NULL);
            }
        }

        printf("Consumer: Consuming an item.....\n");
        data = Buf.buf[Buf.out].data;
        Buf.out = (Buf.out + 1) % MAX_BUF;
        Buf.counter--;

        // 생산자에게 버퍼에 공간이 있다고 신호
        if (pthread_cond_signal(&NotFull) < 0) {
            perror("pthread_cond_signal");
            pthread_exit(NULL);
        }
        if (pthread_mutex_unlock(&Mutex) < 0) {
            perror("pthread_mutex_unlock");
            pthread_exit(NULL);
        }

        ThreadUsleep((rand() % 100) * 10000);
    }

    printf("Consumer: Consumed %d items.....\n", i);
    printf("Consumer: %d items in buffer.....\n", Buf.counter);

    pthread_exit(NULL);
}

char	*data;

int main()
{
	int		shmid;
	pthread_t       tid1, tid2;

	if ((shmid = shmget(SHM_KEY, SHM_SIZE, SHM_MODE)) < 0)  {
		perror("shmget");
		exit(1);
	}

	if (pthread_create(&tid1, NULL, (void *)Producer, NULL) < 0)  {
        perror("pthread_create");
        exit(1);
    }

	if (pthread_create(&tid2, NULL, (void *)Consumer, NULL) < 0)  {
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

	// 공유 메모리 삭제
	if (shmctl(shmid, IPC_RMID, 0) < 0)  {
		perror("shmctl");
		exit(1);
	}

	return 0;
}