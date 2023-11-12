#include <stdio.h>
#include <pthread.h>

#define MAX_CMD 256

// 주어진 명령을 수행하고 완료 메시지를 출력하는 함수
void DoCmd(char *cmd)
{
    printf("Doing %s", cmd);
    sleep(1); // 1초 동안 대기 (명령 수행 시간을 가정)
    printf("Done\n");

    pthread_exit(NULL);
}

main()
{
    char cmd[MAX_CMD];
    pthread_t tid;

    while (1) {
        printf("CMD> "); // 사용자로부터 명령을 입력받기 위한 프롬프트 출력
        fgets(cmd, MAX_CMD, stdin); // 사용자 입력을 cmd 변수에 읽어옴

        // 입력된 명령이 'q'로 시작하면 루프를 종료하여 프로그램 종료
        if (cmd[0] == 'q')
            break;

        if (pthread_create(&tid, NULL, (void *)DoCmd, (void *)cmd) < 0) {
            perror("pthread_create");
            exit(1);
        }

        // 주석 처리한 부분에서 pthread_join 함수를 사용하지 않고 루프를 돌며 쓰레드 생성
        // pthread_join 함수를 사용할 경우, 생성된 쓰레드가 종료될 때까지 기다림
        // 주석 처리한 부분을 사용하면 생성된 쓰레드를 기다리지 않고 계속해서 새로운 쓰레드 생성 가능
        // 주석 처리한 부분을 사용하려면 pthread_join 부분의 주석을 해제해야 함
    }
}
