#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>

// 세마포어 초기화 함수
int semInit(key_t key);

// 세마포어 초기값 설정 함수
int semInitValue(int semid, int value);

// 세마포어 대기 함수
int semWait(int semid);

// 세마포어 시도 대기 함수
int semTryWait(int semid);

// 세마포어 신호 전송 함수
int semPost(int semid);

// 세마포어 값 조회 함수
int semGetValue(int semid);

// 세마포어 제거 함수
int semDestroy(int semid);
