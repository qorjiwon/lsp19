#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <signal.h>
#include "udp.h"
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
    printf("\nUDP Server exit.....\n");
    exit(0);
}

// UDP 서버 메인 함수
main(int argc, char *argv[]) {
    int cliAddrLen, n;
    struct sockaddr_in cliAddr, servAddr;
    MsgType msg;

    // SIGINT 시그널(인터럽트 시그널)이 발생하면 CloseServer 함수 호출
    signal(SIGINT, CloseServer);

    // 소켓 생성
    if ((Sockfd = socket(PF_INET, SOCK_DGRAM, 0)) < 0) {
        perror("socket");
        exit(1);
    }

    // 서버 주소 설정
    bzero((char *)&servAddr, sizeof(servAddr));
    servAddr.sin_family = PF_INET;
    servAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servAddr.sin_port = htons(SERV_UDP_PORT);

    // 소켓에 서버 주소 바인딩
    if (bind(Sockfd, (struct sockaddr *)&servAddr, sizeof(servAddr)) < 0) {
        perror("bind");
        exit(1);
    }

    printf("UDP Server started.....\n");

    cliAddrLen = sizeof(cliAddr);
    while (1) {
        // 클라이언트로부터 메시지 수신
        if ((n = recvfrom(Sockfd, (char *)&msg, sizeof(msg), 0, (struct sockaddr *)&cliAddr, &cliAddrLen)) < 0) {
            perror("recvfrom");
            exit(1);
        }
        printf("Received request: %s.....", msg.data);

        // 메시지 타입을 MSG_REPLY로 설정하고 응답 메시지 작성
        msg.type = MSG_REPLY;
        sprintf(msg.data, "This is a reply from %d.", getpid());

        // 클라이언트에게 응답 메시지 전송
        if (sendto(Sockfd, (char *)&msg, sizeof(msg), 0, (struct sockaddr *)&cliAddr, cliAddrLen) < 0) {
            perror("sendto");
            exit(1);
        }
        printf("Replied.\n");
    }
}

