#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/un.h>
#include <signal.h>
#include <sys/time.h>
#include <unistd.h>
#include "select.h"


int	TcpSockfd;
int	UdpSockfd;
int	UcoSockfd;
int	UclSockfd;
// 서버를 종료하는 함수
void CloseServer()
{
    // 모든 소켓을 닫음
    close(TcpSockfd);
    close(UdpSockfd);
    close(UcoSockfd);
    close(UclSockfd);

    // UNIX 도메인 경로에 대한 파일 제거
    if (remove(UNIX_STR_PATH) < 0)  {
        perror("remove");
    }

    // UNIX 도메인 DG 경로에 대한 파일 제거
    if (remove(UNIX_DG_PATH) < 0)  {
        perror("remove");
    }

    // 종료 메시지 출력
    printf("\n서버 데몬 종료.....\n");

    // 프로그램 종료
    exit(0);
}

// TCP 소켓을 생성하는 함수
void MakeTcpSocket()
{
    struct sockaddr_in servAddr;

    // TCP 소켓 생성
    if ((TcpSockfd = socket(PF_INET, SOCK_STREAM, 0)) < 0)  {
        perror("socket");
        exit(1);
    }

    // 서버 주소 구조체 초기화
    bzero((char *)&servAddr, sizeof(servAddr));
    servAddr.sin_family = PF_INET;
    servAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servAddr.sin_port = htons(SERV_TCP_PORT);

    // 소켓에 주소 할당
    if (bind(TcpSockfd, (struct sockaddr *) &servAddr, sizeof(servAddr)) < 0)  {
        perror("bind");
        exit(1);
    }

    // 연결 대기 상태로 설정
    listen(TcpSockfd, 5);
}

// UDP 소켓을 생성하는 함수
void MakeUdpSocket()
{
    struct sockaddr_in servAddr;

    // UDP 소켓 생성
    if ((UdpSockfd = socket(PF_INET, SOCK_DGRAM, 0)) < 0)  {
        perror("socket");
        exit(1);
    }

    // 서버 주소 구조체 초기화
    bzero((char *)&servAddr, sizeof(servAddr));
    servAddr.sin_family = PF_INET;
    servAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servAddr.sin_port = htons(SERV_UDP_PORT);

    // 소켓에 주소 할당
    if (bind(UdpSockfd, (struct sockaddr *) &servAddr, sizeof(servAddr)) < 0)  {
        perror("bind");
        exit(1);
    }
}
// UNIX 도메인 소켓 (UCO)을 생성하는 함수
void MakeUcoSocket()
{
    struct sockaddr_un servAddr;
    int servAddrLen;

    // UCO 소켓 생성
    if ((UcoSockfd = socket(PF_UNIX, SOCK_STREAM, 0)) < 0)  {
        perror("socket");
        exit(1);
    }

    // 서버 주소 구조체 초기화
    bzero((char *)&servAddr, sizeof(servAddr));
    servAddr.sun_family = PF_UNIX;
    strcpy(servAddr.sun_path, UNIX_STR_PATH);
    servAddrLen = strlen(servAddr.sun_path) + sizeof(servAddr.sun_family);

    // 소켓에 주소 할당
    if (bind(UcoSockfd, (struct sockaddr *) &servAddr, servAddrLen) < 0)  {
        perror("bind");
        exit(1);
    }

    // 연결 대기 상태로 설정
    listen(UcoSockfd, 5);
}

// UNIX 도메인 소켓 (UCL)을 생성하는 함수
void MakeUclSocket()
{
    struct sockaddr_un servAddr;
    int servAddrLen;

    // UCL 소켓 생성
    if ((UclSockfd = socket(PF_UNIX, SOCK_DGRAM, 0)) < 0)  {
        perror("socket");
        exit(1);
    }

    // 서버 주소 구조체 초기화
    bzero((char *)&servAddr, sizeof(servAddr));
    servAddr.sun_family = PF_UNIX;
    strcpy(servAddr.sun_path, UNIX_DG_PATH);
    servAddrLen = strlen(servAddr.sun_path) + sizeof(servAddr.sun_family);

    // 소켓에 주소 할당
    if (bind(UclSockfd, (struct sockaddr *) &servAddr, servAddrLen) < 0)  {
        perror("bind");
        exit(1);
    }
}

// TCP 요청을 처리하는 함수
void ProcessTcpRequest()
{
    int                 newSockfd, cliAddrLen, n;
    struct sockaddr_in  cliAddr;
    MsgType             msg;

    // 클라이언트 주소 구조체의 크기를 가져옴
    cliAddrLen = sizeof(cliAddr);

    // 새로운 연결을 수락
    newSockfd = accept(TcpSockfd, (struct sockaddr *) &cliAddr, &cliAddrLen);
    if (newSockfd < 0)  {
        perror("accept");
        exit(1);
    }
    
    // 수락된 연결에서 메시지를 읽음
    if ((n = read(newSockfd, (char *)&msg, sizeof(msg))) < 0)  {
        perror("read");
        exit(1);
    }
    printf("TCP 요청 받음: %s.....", msg.data);

    // 응답 메시지를 준비
    msg.type = MSG_REPLY;
    sprintf(msg.data, "이것은 %d에서의 응답입니다.", getpid());

    // 수락된 연결에 응답 메시지를 작성
    if (write(newSockfd, (char *)&msg, sizeof(msg)) < 0)  {
        perror("write");
        exit(1);
    }
    printf("응답 완료.\n");

    // 수락된 연결을 닫음
    close(newSockfd);
}

// UDP 요청을 처리하는 함수
void ProcessUdpRequest()
{
    int                 cliAddrLen, n;
    struct sockaddr_in  cliAddr;
    MsgType             msg;

    // 클라이언트 주소 구조체의 크기를 가져옴
    cliAddrLen = sizeof(cliAddr);

    // UDP 소켓에서 메시지를 수신
    if ((n = recvfrom(UdpSockfd, (char *)&msg, sizeof(msg), 
                    0, (struct sockaddr *)&cliAddr, &cliAddrLen)) < 0)  {
        perror("recvfrom");
        exit(1);
    }

    // 수신된 UDP 요청 데이터를 출력
    printf("UDP 요청 받음: %s.....", msg.data);

    // 응답 메시지를 준비
    msg.type = MSG_REPLY;
    sprintf(msg.data, "이것은 %d에서의 응답입니다.", getpid());

    // 클라이언트에 응답 메시지를 다시 보냄
    if (sendto(UdpSockfd, (char *)&msg, sizeof(msg),
                0, (struct sockaddr *)&cliAddr, cliAddrLen) < 0)  {
        perror("sendto");
        exit(1);
    }

    // 응답이 전송되었음을 나타내는 메시지 출력
    printf("응답 완료.\n");
}

// UNIX 도메인 소켓 (UCO) 요청을 처리하는 함수
void ProcessUcoRequest()
{
    int                 newSockfd, cliAddrLen, n;
    struct sockaddr_un  cliAddr;
    MsgType             msg;

    // 클라이언트 주소 구조체의 크기를 가져옴
    cliAddrLen = sizeof(cliAddr);

    // UNIX 도메인 소켓에서 새로운 연결을 수락
    newSockfd = accept(UcoSockfd, (struct sockaddr *) &cliAddr, &cliAddrLen);
    if (newSockfd < 0)  {
        perror("accept");
        exit(1);
    }

    // 수락된 연결에서 메시지를 읽음
    if ((n = read(newSockfd, (char *)&msg, sizeof(msg))) < 0)  {
        perror("read");
        exit(1);
    }

    // 수신된 UNIX 도메인 CO 요청 데이터를 출력
    printf("UNIX 도메인 CO 요청 받음: %s.....", msg.data);

    // 응답 메시지를 준비
    msg.type = MSG_REPLY;
    sprintf(msg.data, "이것은 %d에서의 응답입니다.", getpid());

    // 수락된 연결에 응답 메시지를 작성
    if (write(newSockfd, (char *)&msg, sizeof(msg)) < 0)  {
        perror("write");
        exit(1);
    }

    // 응답이 전송되었음을 나타내는 메시지 출력
    printf("응답 완료.\n");

    // 수락된 연결을 닫음
    close(newSockfd);
}

// UNIX 도메인 CL 요청을 처리하는 함수
void ProcessUclRequest()
{
    int                 cliAddrLen, n;
    struct sockaddr_un  cliAddr;
    MsgType             msg;

    // 클라이언트 주소 구조체의 크기를 가져옴
    cliAddrLen = sizeof(cliAddr);

    // UCL 소켓에서 메시지를 수신
    if ((n = recvfrom(UclSockfd, (char *)&msg, sizeof(msg), 
                0, (struct sockaddr *)&cliAddr, &cliAddrLen)) < 0)  {
        perror("recvfrom");
        exit(1);
    }
    printf("Received UNIX-domain CL request: %s.....", msg.data);

    // 응답 메시지를 준비
    msg.type = MSG_REPLY;
    sprintf(msg.data, "이것은 %d에서의 응답입니다.", getpid());

    // 수신된 클라이언트에게 응답 메시지를 다시 보냄
    if (sendto(UclSockfd, (char *)&msg, sizeof(msg),
                0, (struct sockaddr *)&cliAddr, cliAddrLen) < 0)  {
        perror("sendto");
        exit(1);
    }
    printf("Replied.\n");
}

// 메인 함수
int main(int argc, char *argv[])
{
    fd_set fdvar;
    int count;

    // 프로그램 종료를 위한 시그널 핸들링 설정
    signal(SIGINT, CloseServer);

    // 각각의 소켓을 생성
    MakeTcpSocket();
    MakeUdpSocket();
    MakeUcoSocket();
    MakeUclSocket();

    printf("서버 데몬 시작됨.....\n");

    while (1)  {
        FD_ZERO(&fdvar);
        FD_SET(TcpSockfd, &fdvar);
        FD_SET(UdpSockfd, &fdvar);
        FD_SET(UcoSockfd, &fdvar);
        FD_SET(UclSockfd, &fdvar);

        // 소켓 이벤트를 기다림
        if ((count = select(10, &fdvar, (fd_set *)NULL, (fd_set *)NULL, 
            (struct timeval *)NULL)) < 0)  {
            perror("select");
            exit(1);
        }

        // 이벤트가 발생한 소켓에 대해 처리
        while (count--)  {
            if (FD_ISSET(TcpSockfd, &fdvar))  {
                ProcessTcpRequest();
            }
            else if (FD_ISSET(UdpSockfd, &fdvar))  {
                ProcessUdpRequest();
            }
            else if (FD_ISSET(UcoSockfd, &fdvar))  {
                ProcessUcoRequest();
            }
            else if (FD_ISSET(UclSockfd, &fdvar))  {
                ProcessUclRequest();
            }
        }
    }
    return 0; // 이 부분에 도달하면 없지만, 표준에 맞춰 0을 반환
}