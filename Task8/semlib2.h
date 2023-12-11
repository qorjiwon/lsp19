#include <pthread.h>

// 세마포어 구조체 정의
typedef struct {
    int sval;              // 세마포어 값
    pthread_mutex_t mutex; // 뮤텍스
    pthread_cond_t cond;   // 조건 변수
} sem_t;

// 세마포어 초기화 함수
int sem_init(sem_t *sem, int pshared, int value);

// 세마포어 대기 함수
int sem_wait(sem_t *sem);

// 세마포어 시도 대기 함수
int sem_trywait(sem_t *sem);

// 세마포어 신호 발생 함수
int sem_post(sem_t *sem);

// 세마포어 값 조회 함수
int sem_getvalue(sem_t *sem, int *sval);

// 세마포어 소멸 함수
int sem_destroy(sem_t *sem);

