#include <stdio.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include "msg.h"
#include <stdlib.h>
#include <unistd.h>

#define	MSG_MODE	(0600 | IPC_CREAT)


int main()
{
	int		msqid, n;
	MsgType	msg;

	// 메시지 큐 생성 또는 열기
	if ((msqid = msgget(MSG_KEY, MSG_MODE)) < 0)  {
		perror("msgget");
		exit(1);
	}

	// 메시지 큐에서 메시지 수신
	if ((n = msgrcv(msqid, (char *)&msg, sizeof(msg), 0, 0)) < 0)  {
		perror("msgrcv");
		exit(1);
	}
	printf("Received request: %s.....", msg.data);

	// 수신한 요청에 대한 응답 메시지 생성 및 메시지 큐에 전송
	msg.type = MSG_REPLY;
	sprintf(msg.data, "This is a reply from %d.", getpid());
	if (msgsnd(msqid, (char *)&msg, sizeof(msg), 0) < 0)  {
		perror("msgsnd");
		exit(1);
	}
	printf("Replied.\n");

	// 일시적인 딜레이 (1초)
	sleep(1);


    	// 메시지 큐 삭제
	if (msgctl(msqid, IPC_RMID, 0) < 0)  {
		perror("msgctl");
		exit(1);
	}
	return 0;
}
