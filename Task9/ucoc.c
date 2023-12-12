#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/un.h>
#include "unix.h"
#include <unistd.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>

// UNIX 도메인 소켓 통신을 위한 클라이언트 메인 함수
main(int argc, char *argv[]) {
    int sockfd, n, servAddrLen;
    struct sockaddr_un servAddr;
    MsgType msg;

    // 소켓 생성
    if ((sockfd = socket(PF_UNIX, SOCK_STREAM, 0)) < 0) {
        perror("socket");
        exit(1);
    }

    // 서버 주소 설정
    bzero((char *)&servAddr, sizeof(servAddr));
    servAddr.sun_family = PF_UNIX;
    strcpy(servAddr.sun_path, UNIX_STR_PATH);
    servAddrLen = strlen(servAddr.sun_path) + sizeof(servAddr.sun_family);

    // 서버에 연결
    if (connect(sockfd, (struct sockaddr *)&servAddr, servAddrLen) < 0) {
        perror("connect");
        exit(1);
    }

    // 메시지 생성 및 서버에 전송
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
}

