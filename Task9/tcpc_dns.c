#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include "tcp.h"
#include <unistd.h>
#include <stdlib.h>

// TCP 클라이언트의 메인 함수
main(int argc, char *argv[]) {
    int sockfd, n;
    struct sockaddr_in servAddr;
    MsgType msg;
    struct hostent *hp;

    // 명령행 인수가 부족한 경우 사용 방법을 출력하고 종료
    if (argc != 2) {
        fprintf(stderr, "Usage: %s IPaddress\n", argv[0]);
        exit(1);
    }

    // 소켓 생성
    if ((sockfd = socket(PF_INET, SOCK_STREAM, 0)) < 0) {
        perror("socket");
        exit(1);
    }

    // 서버 주소 초기화
    bzero((char *)&servAddr, sizeof(servAddr));
    servAddr.sin_family = PF_INET;
    servAddr.sin_port = htons(SERV_TCP_PORT);

    // 입력된 인수가 숫자인 경우 IP 주소로 처리, 아닌 경우 호스트 이름으로 처리
    if (isdigit(argv[1][0])) {
        servAddr.sin_addr.s_addr = inet_addr(argv[1]);
    } else {
        // 호스트 이름을 IP 주소로 변환
        if ((hp = gethostbyname(argv[1])) == NULL) {
            fprintf(stderr, "Unknown host: %s\n", argv[1]);
            exit(1);
        }
        memcpy(&servAddr.sin_addr, hp->h_addrtype, hp->h_length);
    }

    // 서버에 연결
    if (connect(sockfd, (struct sockaddr *)&servAddr, sizeof(servAddr)) < 0) {
        perror("connect");
        exit(1);
    }

    // 요청 메시지 생성 및 서버에 전송
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