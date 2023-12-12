#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <signal.h>
#include "sg.h"
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

// 전역 변수로 소켓 파일 디스크립터를 선언
int Sockfd;

// 서버 종료 시 호출되는 함수
void CloseServer()
{
    close(Sockfd);
    printf("\nScatter/Gather TCP Server exit.....\n");
    exit(0);
}

// 메인 함수
int main(int argc, char *argv[])
{
    int newSockfd, cliAddrLen, n;
    struct sockaddr_in cliAddr, servAddr;
    MsgType msg;
    HeaderType hdr;
    struct iovec iov[2];

    // SIGINT 시그널(인터럽트 시그널)을 받았을 때 CloseServer 함수 호출
    signal(SIGINT, CloseServer);

    // 소켓 생성
    if ((Sockfd = socket(PF_INET, SOCK_STREAM, 0)) < 0)
    {
        perror("socket");
        exit(1);
    }

    // 서버 주소 초기화
    bzero((char *)&servAddr, sizeof(servAddr));
    servAddr.sin_family = PF_INET;
    servAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servAddr.sin_port = htons(SERV_TCP_PORT);

    // 소켓에 서버 주소를 바인딩
    if (bind(Sockfd, (struct sockaddr *)&servAddr, sizeof(servAddr)) < 0)
    {
        perror("bind");
        exit(1);
    }

    // 연결 대기열 생성
    listen(Sockfd, 5);

    printf("Scatter/Gather TCP Server started.....\n");

    cliAddrLen = sizeof(cliAddr);

    // 클라이언트의 연결을 수락하고 처리하는 무한 루프
    while (1)
    {
        // 클라이언트로부터 연결을 수락하고, 연결된 소켓의 파일 디스크립터를 반환
        newSockfd = accept(Sockfd, (struct sockaddr *)&cliAddr, &cliAddrLen);
        if (newSockfd < 0)
        {
            perror("accept");
            exit(1);
        }

        // iov 배열을 사용하여 헤더와 메시지를 받음
        iov[0].iov_base = (char *)&hdr;
        iov[0].iov_len = sizeof(hdr);
        iov[1].iov_base = (char *)&msg;
        iov[1].iov_len = sizeof(msg);

        // readv 함수를 사용하여 iov 배열에 대응하는 데이터를 읽음
        if ((n = readv(newSockfd, iov, 2)) < 0)
        {
            perror("read");
            exit(1);
        }

        // 받은 요청 출력
        printf("Received request: %s(%s).....", msg.data, hdr.info);

        // 응답 생성
        strcpy(hdr.info, "REPLY");
        msg.type = MSG_REPLY;
        sprintf(msg.data, "This is a reply from %d.", getpid());

        // writev 함수를 사용하여 iov 배열에 대응하는 데이터를 쓰기
        if (writev(newSockfd, iov, 2) < 0)
        {
            perror("write");
            exit(1);
        }

        printf("Replied.\n");

        // 연결된 소켓 닫기
        close(newSockfd);
    }

    return 0;
}

