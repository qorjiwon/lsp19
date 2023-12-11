#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <semaphore.h>
#include <pthread.h>
#include <sys/time.h>

#define NUM_MEN 5
#define NLOOPS 5

sem_t ChopStick[NUM_MEN];

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

void Thinking(int id) {
    printf("Philosopher%d: Thinking.....\n", id);
    ThreadUsleep((rand() % 200) * 10000);
    printf("Philosopher%d: Want to eat.....\n", id);
}

void Eating(int id) {
    printf("Philosopher%d: Eating.....\n", id);
    ThreadUsleep((rand() % 100) * 10000);
}

void DiningPhilosopher(int *pId) {
    int i;
    int id = *pId;

    for (i = 0; i < NLOOPS; i++) {

        Thinking(id);

        // 왼쪽 젓가락 세마포어 대기
        if (sem_wait(&ChopStick[id]) < 0) {
            perror("sem_wait");
            pthread_exit(NULL);
        }
        // 오른쪽 젓가락 세마포어 대기
        if (sem_wait(&ChopStick[(id + 1) % NUM_MEN]) < 0) {
            perror("sem_wait");
            pthread_exit(NULL);
        }

        Eating(id);

        // 왼쪽 젓가락 세마포어 신호 전송
        if (sem_post(&ChopStick[id]) < 0) {
            perror("sem_post");
            pthread_exit(NULL);
        }
        // 오른쪽 젓가락 세마포어 신호 전송
        if (sem_post(&ChopStick[(id + 1) % NUM_MEN]) < 0) {
            perror("sem_post");
            pthread_exit(NULL);
        }
    }

    printf("Philosopher%d: thinking & eating %d times.....\n", id, i);

    pthread_exit(NULL);
}

int main() {
    pthread_t tid[NUM_MEN];
    int i, id[NUM_MEN];

    srand(0x8888);

    // 세마포어 초기화
    for (i = 0; i < NUM_MEN; i++) {
        if (sem_init(&ChopStick[i], 0, 1) < 0) {
            perror("sem_init");
            exit(1);
        }
        id[i] = i;
    }

    // 철학자 쓰레드 생성
    for (i = 0; i < NUM_MEN; i++) {
        if (pthread_create(&tid[i], NULL, (void *)DiningPhilosopher, (void *)&id[i]) < 0) {
            perror("pthread_create");
            exit(1);
        }
    }

    // 쓰레드 종료 대기
    for (i = 0; i < NUM_MEN; i++) {
        if (pthread_join(tid[i], NULL) < 0) {
            perror("pthread_join");
            exit(1);
        }
    }

    // 세마포어 해제
    for (i = 0; i < NUM_MEN; i++) {
        if (sem_destroy(&ChopStick[i]) < 0) {
            perror("sem_destroy");
        }
    }

    return 0;
}
