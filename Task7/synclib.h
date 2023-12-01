#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>

void TELL_WAIT(void); // pipe 생성
void TELL_PARENT(void); // 부모 프로세스에게 알림
void WAIT_PARENT(void); // 부모 프로세스가 자식 프로세스의 알림을 기다림
void TELL_CHILD(void); // 자식 프로세스에게 알림
void WAIT_CHILD(void); // 자식 프로세스가 부모 프로세스의 알림을 기다림
