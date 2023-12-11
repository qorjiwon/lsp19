#include <stdio.h>
#include <pthread.h>
#include "semlib2.h"

// 세마포어 초기화 함수
int sem_init(sem_t *sem, int pshared, int value)
{
    // pshared가 0이 아니면 지원하지 않음
    if (pshared)
    {
        fprintf(stderr, "Function not supported\n");
        return -1;
    }

    // 뮤텍스 초기화
    if (pthread_mutex_init(&sem->mutex, NULL) < 0)
        return -1;

    // 조건 변수 초기화
    if (pthread_cond_init(&sem->cond, NULL) < 0)
        return -1;

    // 세마포어 값 설정
    sem->sval = value;

    return 0;
}

// 세마포어 대기 함수
int sem_wait(sem_t *sem)
{
    // 뮤텍스 락
    if (pthread_mutex_lock(&sem->mutex) < 0)
        return -1;

    // 세마포어 값이 0인 경우 대기
    if (sem->sval == 0)
    {
        while (sem->sval == 0)
        {
            // 대기하다가 시그널을 받으면 깨어나고 세마포어 값을 감소
            if (pthread_cond_wait(&sem->cond, &sem->mutex) < 0)
            {
                // 대기 중 에러 발생 시 뮤텍스 언락 후 에러 반환
                if (pthread_mutex_unlock(&sem->mutex) < 0)
                    return -1;
                return -1;
            }
        }
        sem->sval--;
    }
    else
    {
        // 세마포어 값이 0이 아닌 경우 감소
        sem->sval--;
    }

    // 뮤텍스 언락
    if (pthread_mutex_unlock(&sem->mutex) < 0)
        return -1;

    return 0;
}

// 세마포어 시도 대기 함수
int sem_trywait(sem_t *sem)
{
    // 뮤텍스 락
    if (pthread_mutex_lock(&sem->mutex) < 0)
        return -1;

    // 세마포어 값이 0인 경우 에러 반환
    if (sem->sval == 0)
    {
        if (pthread_mutex_unlock(&sem->mutex) < 0)
            return -1;
        return -1;
    }
    else
    {
        // 세마포어 값이 0이 아닌 경우 감소
        sem->sval--;
    }

    // 뮤텍스 언락
    if (pthread_mutex_unlock(&sem->mutex) < 0)
        return -1;

    return 0;
}

// 세마포어 신호 함수
int sem_post(sem_t *sem)
{
    // 뮤텍스 락
    if (pthread_mutex_lock(&sem->mutex) < 0)
        return -1;

    // 세마포어 값이 0인 경우 신호 전달
    if (sem->sval == 0)
    {
        if (pthread_cond_signal(&sem->cond) < 0)
        {
            // 신호 전달 중 에러 발생 시 뮤텍스 언락 후 에러 반환
            if (pthread_mutex_unlock(&sem->mutex) < 0)
                return -1;
            return -1;
        }
    }

    // 세마포어 값 증가
    sem->sval++;

    // 뮤텍스 언락
    if (pthread_mutex_unlock(&sem->mutex) < 0)
        return -1;

    return 0;
}

// 세마포어 값 조회 함수
int sem_getvalue(sem_t *sem, int *sval)
{
    // 현재 세마포어 값 조회
    *sval = sem->sval;

    return 0;
}

// 세마포어 소멸 함수
int sem_destroy(sem_t *sem)
{
    // 뮤텍스 소멸
    if (pthread_mutex_destroy(&sem->mutex) < 0)
        return -1;

    // 조건 변수 소멸
    if (pthread_cond_destroy(&sem->cond) < 0)
        return -1;

    return 0;
}
