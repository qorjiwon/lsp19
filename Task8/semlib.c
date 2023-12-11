#include <stdio.h>
#include "semlib.h"

/*
 * 세마포어 초기화 함수
 * key: 세마포어를 식별하는 데 사용되는 키
 * 반환값: 생성된 세마포어의 식별자, 실패 시 -1
 */
int semInit(key_t key) {
    int semid;

    // 세마포어 생성 및 식별자 획득
    if ((semid = semget(key, 1, 0600 | IPC_CREAT)) < 0)  {
        perror("semget");
        return -1;
    }

    return semid;
}

/*
 * 세마포어 초기값 설정 함수
 * semid: 초기값을 설정할 세마포어의 식별자
 * value: 설정할 초기값
 * 반환값: 설정된 세마포어의 식별자, 실패 시 -1
 */
int semInitValue(int semid, int value) {
    union semun {
        int val;
    } semun;

    // 세마포어 초기값 설정
    semun.val = value;
    if (semctl(semid, 0, SETVAL, semun) < 0)  {
        perror("semctl");
        return -1;
    }

    return semid;
}

/*
 * 세마포어 대기 함수
 * semid: 대기할 세마포어의 식별자
 * 반환값: 성공 시 0, 실패 시 -1
 */
int semWait(int semid) {
    struct sembuf semcmd;

    // 세마포어 대기 조작 수행
    semcmd.sem_num = 0;
    semcmd.sem_op = -1;
    semcmd.sem_flg = SEM_UNDO;
    if (semop(semid, &semcmd, 1) < 0)  {
        perror("semop");
        return -1;
    }

    return 0;
}

/*
 * 세마포어 시도 대기 함수
 * semid: 대기할 세마포어의 식별자
 * 반환값: 성공 시 0, 실패 시 -1
 */
int semTryWait(int semid) {
    struct sembuf semcmd;

    // 세마포어 시도 대기 조작 수행
    semcmd.sem_num = 0;
    semcmd.sem_op = -1;
    semcmd.sem_flg = IPC_NOWAIT | SEM_UNDO;
    if (semop(semid, &semcmd, 1) < 0)  {
        perror("semop");
        return -1;
    }

    return 0;
}

/*
 * 세마포어 신호 전송 함수
 * semid: 신호 전송할 세마포어의 식별자
 * 반환값: 성공 시 0, 실패 시 -1
 */
int semPost(int semid) {
    struct sembuf semcmd;

    // 세마포어 신호 전송 조작 수행
    semcmd.sem_num = 0;
    semcmd.sem_op = 1;
    semcmd.sem_flg = SEM_UNDO;
    if (semop(semid, &semcmd, 1) < 0)  {
        perror("semop");
        return -1;
    }

    return 0;
}

/*
 * 세마포어 값 조회 함수
 * semid: 조회할 세마포어의 식별자
 * 반환값: 세마포어의 현재 값
 */
int semGetValue(int semid) {
    union semun {
        int val;
    } dummy;

    // 세마포어 값 조회
    return semctl(semid, 0, GETVAL, dummy);
}

/*
 * 세마포어 제거 함수
 * semid: 제거할 세마포어의 식별자
 * 반환값: 성공 시 0, 실패 시 -1
 */
int semDestroy(int semid) {
    union semun {
        int val;
    } dummy;

    // 세마포어 제거
    if (semctl(semid, 0, IPC_RMID, dummy) < 0)  {
        perror("semctl");
        return -1;
    }

    return 0;
}
