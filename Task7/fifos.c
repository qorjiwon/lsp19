#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <signal.h>
#include <errno.h>
#include "fifo.h"
#include <stdlib.h>

// SIGINT 시그널 핸들러: 서버 종료 시 FIFO 파일을 제거하고 프로그램을 종료
void SigIntHandler(int signo)
{
	if (remove(SERV_FIFO) < 0)  {
		perror("remove");
		exit(1);
	}

	exit(0);
}


int main()
{
	int			fd, cfd, n;
	MsgType		msg;

	// SIGINT 시그널에 대한 핸들러 등록
	if (signal(SIGINT, SigIntHandler) == SIG_ERR)  {
		perror("signal");
		exit(1);
	}

	// 서버 FIFO 파일 생성 (이미 존재하는 경우 무시)
	if (mkfifo(SERV_FIFO, 0600) < 0)  {
		if (errno != EEXIST)  {
			perror("mkfifo");
			exit(1);
		}
	}

	// 서버 FIFO 파일 열기 (읽고 쓰기 권한)
	if ((fd = open(SERV_FIFO, O_RDWR)) < 0)  {
		perror("open");
		exit(1);
	}

	// 클라이언트로부터 메시지를 받아 응답
	while (1)  {
		if ((n = read(fd, (char *)&msg, sizeof(msg))) < 0)  {
			if (errno == EINTR)  {
				continue; // 인터럽트(CTRL+C 등)가 발생한 경우 계속 수행
			}
			else  {
				perror("read");
				exit(1);
			}
		}
		printf("Received request: %s.....", msg.data);

		// 클라이언트로부터 받은 FIFO 파일 열기 (쓰기 권한)
		if ((cfd = open(msg.returnFifo, O_WRONLY)) < 0)  {
			perror("open");
			exit(1);
		}

		// 클라이언트에게 응답 메시지 전송
		sprintf(msg.data, "This is a reply from %d.", getpid());
		write(cfd, (char *)&msg, sizeof(msg));
		close(cfd);
		printf("Replied.\n");
	}
	return 0;
}
