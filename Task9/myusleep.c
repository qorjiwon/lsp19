#include <stdio.h>
#include <sys/types.h>
#include <sys/time.h>
#include <unistd.h>

// 사용자 정의 usleep 함수
void myusleep(int secs, int usecs) {
    struct timeval tm;

    tm.tv_sec = secs;
    tm.tv_usec = usecs;

    // select를 사용하여 지정된 시간만큼 대기
    if (select(0, (fd_set *)0, (fd_set *)0, (fd_set *)0, &tm) < 0) {
        perror("select");
        return;
    }
}

// 메인 함수
main() {
    printf("Sleep for 1.5 seconds.....\n");
    
    // 1.5초 동안 대기
    myusleep(1, 500000);
}

