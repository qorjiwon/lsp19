#include <stdio.h>
#include <pthread.h>

// 스레드에서 실행할 함수로, 메시지를 출력합니다.
void PrintMsg(char *msg)
{
    printf("%s", msg);

    pthread_exit(NULL);
}

void main()
{
    pthread_t tid1, tid2;   // 스레드 식별자를 저장할 변수
    char *msg1 = "Hello, ";  // 스레드 1이 출력할 메시지
    char *msg2 = "World!\n"; // 스레드 2이 출력할 메시지

    // 첫 번째 스레드를 생성하고, PrintMsg 함수를 실행합니다.
    if (pthread_create(&tid1, NULL, (void *)PrintMsg, (void *)msg1) < 0) {
        perror("pthread_create"); // 스레드 생성 오류 시 에러 출력
        exit(1);
    }

    // 두 번째 스레드를 생성하고, PrintMsg 함수를 실행합니다.
    if (pthread_create(&tid2, NULL, (void *)PrintMsg, (void *)msg2) < 0) {
        perror("pthread_create");
        exit(1);
    }

    printf("Threads created: tid=%lu, %lu\n", tid1, tid2);
    
    // 각 스레드가 종료될 때까지 대기합니다.
    if (pthread_join(tid1, NULL) < 0) {
        perror("pthread_join"); // 스레드 대기 오류 시 에러 출력
        exit(1);
    }
    if (pthread_join(tid2, NULL) < 0) {
        perror("pthread_join");
        exit(1);
    }

    printf("Threads terminated: tid=%d, %d\n", tid1, tid2);
}
