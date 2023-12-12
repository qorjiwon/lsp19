#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "udp.h"
#include <unistd.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>

// UDP 클라이언트 메인 함수
main(int argc, char *argv[]) {
    int sockfd, n, peerAddrLen;
    struct sockaddr_in servAddr, peerAddr;
    MsgType msg;

    // 소켓 생성
    if ((sockfd = socket(PF_INET, SOCK_DGRAM, 0)) < 0) {
        perror("socket");
        exit(1);
    }

    // 서버 주소 설정
    bzero((char *)&servAddr, sizeof(servAddr));
    servAddr.sin_family = PF_INET;
    servAddr.sin_addr.s_addr = inet_addr(SERV_HOST_ADDR);
    servAddr.sin_port = htons(SERV_UDP_PORT);

    // 메시지 생성 및 서버에 전송
    msg.type = MSG_REQUEST;
    sprintf(msg.data, "This is a request from %d.", getpid());
    if (sendto(sockfd, (char *)&msg, sizeof(msg), 0, (struct sockaddr *)&servAddr, sizeof(servAddr)) < 0) {
        perror("sendto");
        exit(1);
    }
    printf("Sent a request.....");

    // 서버로부터 응답 메시지 수신
    peerAddrLen = sizeof(peerAddr);
    if ((n = recvfrom(sockfd, (char *)&msg, sizeof(msg), 0, (struct sockaddr *)&peerAddr, &peerAddrLen)) < 0) {
        perror("recvfrom");
        exit(1);
    }
    printf("Received reply: %s\n", msg.data);

    // 소켓 닫기
    close(sockfd);
}