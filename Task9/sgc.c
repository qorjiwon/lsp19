#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "sg.h"
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

// 메인 함수
main(int argc, char *argv[])
{
    int sockfd, n;
    struct sockaddr_in servAddr;
    MsgType msg;
    HeaderType hdr;
    struct iovec iov[2];

    // 소켓 생성
    if ((sockfd = socket(PF_INET, SOCK_STREAM, 0)) < 0)
    {
        perror("socket");
        exit(1);
    }

    // 서버 주소 초기화
    bzero((char *)&servAddr, sizeof(servAddr));
    servAddr.sin_family = PF_INET;
    servAddr.sin_addr.s_addr = inet_addr(SERV_HOST_ADDR); // 서버의 IP 주소
    servAddr.sin_port = htons(SERV_TCP_PORT);             // 서버의 포트 번호

    // 서버에 연결
    if (connect(sockfd, (struct sockaddr *)&servAddr, sizeof(servAddr)) < 0)
    {
        perror("connect");
        exit(1);
    }

    // 헤더와 메시지 정보 설정
    strcpy(hdr.info, "REQST");
    msg.type = MSG_REQUEST;
    sprintf(msg.data, "This is a request from %d.", getpid());

    // iov 배열을 사용하여 헤더와 메시지를 하나의 연속된 데이터로 보냄
    iov[0].iov_base = (char *)&hdr;
    iov[0].iov_len = sizeof(hdr);
    iov[1].iov_base = (char *)&msg;
    iov[1].iov_len = sizeof(msg);

    // writev 함수를 사용하여 iov 배열에 대응하는 데이터를 서버로 전송
    if (writev(sockfd, iov, 2) < 0)
    {
        perror("write");
        exit(1);
    }

    printf("Sent a request.....");

    // 서버에서 받은 응답을 iov 배열을 사용하여 읽음
    if ((n = readv(sockfd, iov, 2)) < 0)
    {
        perror("read");
        exit(1);
    }

    // 받은 응답 출력
    printf("Received reply: %s(%s)\n", msg.data, hdr.info);

    // 소켓 닫기
    close(sockfd);
}
