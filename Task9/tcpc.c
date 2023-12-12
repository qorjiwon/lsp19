#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "tcp.h"
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>

int main(int argc, char *argv[]) {
    int sockfd, n;
    struct sockaddr_in servAddr;
    MsgType msg;

    // 소켓 생성
    if ((sockfd = socket(PF_INET, SOCK_STREAM, 0)) < 0) {
        perror("socket");
        exit(1);
    }

    // 서버 주소 설정
    bzero((char *)&servAddr, sizeof(servAddr));  // bzero 함수를 사용하여 메모리 초기화
    servAddr.sin_family = PF_INET;
    servAddr.sin_addr.s_addr = inet_addr(SERV_HOST_ADDR);
    servAddr.sin_port = htons(SERV_TCP_PORT);

    // 서버에 연결
    if (connect(sockfd, (struct sockaddr *)&servAddr, sizeof(servAddr)) < 0) {
        perror("connect");
        exit(1);
    }

    // 메시지 생성 및 전송
    msg.type = MSG_REQUEST;
    sprintf(msg.data, "This is a request from %d.", getpid());
    if (write(sockfd, (char *)&msg, sizeof(msg)) < 0) {
        perror("write");
        exit(1);
    }
    printf("Sent a request.....");

    // 서버로부터 응답 메시지 수신
    if ((n = read(sockfd, (char *)&msg, sizeof(msg))) < 0) {
        perror("read");
        exit(1);
    }
    printf("Received reply: %s\n", msg.data);

    // 소켓 닫기
    close(sockfd);

    return 0;  // main 함수의 반환값 추가
}

