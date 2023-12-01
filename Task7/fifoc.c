#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include "fifo.h"
#include <stdlib.h>
#include <string.h>

int main()
{
	char		fname[MAX_FIFO_NAME]; // 클라이언트 자신의 FIFO 파일 이름을 저장할 배열
	int			fd, sfd, n;
	MsgType		msg;

	// 클라이언트 자신의 FIFO 파일 이름 생성
	sprintf(fname, ".fifo%d", getpid());

	// 클라이언트 자신의 FIFO 파일 생성
	if (mkfifo(fname, 0600) < 0)  {
		perror("mkfifo");
		exit(1);
	}

	// 클라이언트 자신의 FIFO 파일 열기 (읽고 쓰기 권한)
	if ((fd = open(fname, O_RDWR)) < 0)  {
		perror("open");
		exit(1);
	}

	// 서버 FIFO 파일 열기 (읽고 쓰기 권한)
	if ((sfd = open(SERV_FIFO, O_RDWR)) < 0)  {
		perror("open");
		exit(1);
	}

	// 서버에게 요청 메시지 전송
	strcpy(msg.returnFifo, fname);
	sprintf(msg.data, "This is a request from %d.", getpid());
	write(sfd, (char *)&msg, sizeof(msg));
	printf("Sent a request.....");

	// 서버로부터 응답 메시지 수신
	if ((n = read(fd, (char *)&msg, sizeof(msg))) < 0)  {
		perror("read");
		exit(1);
	}

	printf("Received reply: %s\n", msg.data);

	close(fd);
	close(sfd);

	// 클라이언트 자신의 FIFO 파일 제거
	if (remove(fname) < 0)  {
		perror("remove");
		exit(1);
	}
	return 0;
}
