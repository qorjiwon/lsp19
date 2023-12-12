#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/un.h>
#include <signal.h>
#include "unix.h"
#include <unistd.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>

// 전역 변수로 소켓 디스크립터 선언
int Sockfd;

// 서버 종료 시 호출되는 함수
void CloseServer() {
    close(Sockfd);
    
    // 소켓 파일 제거
    if (remove(UNIX_STR_PATH) < 0) {
        perror("remove");
    }

    printf("\nUNIX-domain Connection-Oriented Server exit.....\n");

    exit(0);
}

// UNIX 도메인 소켓 통신을 위한 서버 메인 함수
main(int argc, char *argv[]) {
    int newSockfd, servAddrLen, cliAddrLen, n;
    struct sockaddr_un cliAddr, servAddr;
    MsgType msg;

    // SIGINT 시그널(인터럽트 시그널)이 발생하면 CloseServer 함수 호출
    signal(SIGINT, CloseServer);

    // 소켓 생성
    if ((Sockfd = socket(PF_UNIX, SOCK_STREAM, 0)) < 0) {
        perror("socket");
        exit(1);
    }

    // 서버 주소 설정
    bzero((char *)&servAddr, sizeof(servAddr));
    servAddr.sun_family = PF_UNIX;
    strcpy(servAddr.sun_path, UNIX_STR_PATH);
    servAddrLen = strlen(servAddr.sun_path) + sizeof(servAddr.sun_family);

    // 소켓에 서버 주소 바인딩
    if (bind(Sockfd, (struct sockaddr *)&servAddr, servAddrLen) < 0) {
        perror("bind");
        exit(1);
    }

    // 연결 요청 대기 상태로 변경
    listen(Sockfd, 5);

    printf("UNIX-domain Connection-Oriented Server started.....\n");

    cliAddrLen = sizeof(cliAddr);
    while (1) {
        // 클라이언트로부터 연결 요청 수락
        newSockfd = accept(Sockfd, (struct sockaddr *)&cliAddr, &cliAddrLen);
        if (newSockfd < 0) {
            perror("accept");
            exit(1);
        }

        // 클라이언트로부터 메시지 수신
        if ((n = read(newSockfd, (char *)&msg, sizeof(msg))) < 0) {
            perror("read");
            exit(1);
        }
        printf("Received request: %s.....", msg.data);

        // 메시지 타입을 MSG_REPLY로 설정하고 응답 메시지 작성
        msg.type = MSG_REPLY;
        sprintf(msg.data, "This is a reply from %d.", getpid());

        // 클라이언트에게 응답 메시지 전송
        if (write(newSockfd, (char *)&msg, sizeof(msg)) < 0) {
            perror("write");
            exit(1);
        }
        printf("Replied.\n");

        // 연결된 소켓 닫기
        close(newSockfd);
    }
}
