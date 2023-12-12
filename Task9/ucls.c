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
#include <sys/types.h>

int Sockfd;

// UNIX 도메인 Connection-Less 서버를 닫는 함수
void CloseServer() {
    close(Sockfd);
    if (remove(UNIX_DG_PATH) < 0) {
        perror("remove");
    }

    printf("\nUNIX-domain Connection-Less Server exit.....\n");

    exit(0);
}

// UNIX 도메인 Connection-Less 서버의 메인 함수
main(int argc, char *argv[]) {
    int Sockfd, servAddrLen, cliAddrLen, n;
    struct sockaddr_un cliAddr, servAddr;
    MsgType msg;

    // 시그널 핸들러 등록
    signal(SIGINT, CloseServer);

    // 소켓 생성
    if ((Sockfd = socket(PF_UNIX, SOCK_DGRAM, 0)) < 0) {
        perror("socket");
        exit(1);
    }

    // 서버 주소 설정
    bzero((char *)&servAddr, sizeof(servAddr));
    servAddr.sun_family = PF_UNIX;
    strcpy(servAddr.sun_path, UNIX_DG_PATH);
    servAddrLen = strlen(servAddr.sun_path) + sizeof(servAddr.sun_family);

    // 바인딩
    if (bind(Sockfd, (struct sockaddr *)&servAddr, servAddrLen) < 0) {
        perror("bind");
        exit(1);
    }

    printf("UNIX-domain Connection-Less Server started.....\n");

    cliAddrLen = sizeof(cliAddr);
    while (1) {
        // 클라이언트로부터 요청 수신
        if ((n = recvfrom(Sockfd, (char *)&msg, sizeof(msg),
                          0, (struct sockaddr *)&cliAddr, &cliAddrLen)) < 0) {
            perror("recvfrom");
            exit(1);
        }
        printf("Received request: %s.....", msg.data);

        // 응답 메시지 생성 및 클라이언트에 전송
        msg.type = MSG_REPLY;
        sprintf(msg.data, "This is a reply from %d.", getpid());
        if (sendto(Sockfd, (char *)&msg, sizeof(msg),
                   0, (struct sockaddr *)&cliAddr, cliAddrLen) < 0) {
            perror("sendto");
            exit(1);
        }
        printf("Replied.\n");
    }
}

