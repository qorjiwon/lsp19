#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <signal.h>
#include "tcp.h"
#include <unistd.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>

// 전역 변수로 소켓 디스크립터를 선언
int Sockfd;

// 서버 종료 시 호출되는 함수
void CloseServer() {
    close(Sockfd);
    printf("\nTCP Server exit.....\n");
    exit(0);
}

// 메인 함수
int main(int argc, char *argv[]) {
    int newSockfd, cliAddrLen, n;
    struct sockaddr_in cliAddr, servAddr;
    MsgType msg;
    pid_t pid;

    // SIGINT 시그널(인터럽트 시그널)이 발생하면 CloseServer 함수 호출
    signal(SIGINT, CloseServer);

    // 소켓 생성
    if ((Sockfd = socket(PF_INET, SOCK_STREAM, 0)) < 0) {
        perror("socket");
        exit(1);
    }

    // 서버 주소 설정
    bzero((char *)&servAddr, sizeof(servAddr));
    servAddr.sin_family = PF_INET;
    servAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servAddr.sin_port = htons(SERV_TCP_PORT);

    // 소켓에 서버 주소 바인딩
    if (bind(Sockfd, (struct sockaddr *)&servAddr, sizeof(servAddr)) < 0) {
        perror("bind");
        exit(1);
    }

    // 클라이언트 연결 대기 상태로 전환
    listen(Sockfd, 5);

    printf("TCP Server started.....\n");

    cliAddrLen = sizeof(cliAddr);
    while (1) {
        // 클라이언트의 연결을 받아들임
        newSockfd = accept(Sockfd, (struct sockaddr *)&cliAddr, &cliAddrLen);
        if (newSockfd < 0) {
            perror("accept");
            exit(1);
        }

        if ((pid = fork()) < 0){
            perror("fork");
            exit(1);
        }
        else if (pid == 0)  { // child
            // 클라이언트로부터 메시지 읽기
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

            // 10,000 마이크로초(10 밀리초) 동안 대기 후 소켓 닫기
            usleep(10000);
            close(newSockfd);
            exit(0);
        }
    }
    return 0;
}