#include <stdio.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include "msg.h"
#include <stdlib.h>
#include <unistd.h>

#define	MSG_MODE	(0600)


int main()
{
	int		msqid, n;
	MsgType	msg;

	// 메시지 큐 열기
	if ((msqid = msgget(MSG_KEY, MSG_MODE)) < 0)  {
		perror("msgget");
		exit(1);
	}

	// 요청 메시지 생성 및 메시지 큐에 전송
	msg.type = MSG_REQUEST;
	sprintf(msg.data, "This is a request from %d.", getpid());
	if (msgsnd(msqid, (char *)&msg, sizeof(msg), 0) < 0)  {
		perror("msgsnd");
		exit(1);
	}
	printf("Sent a request.....");

	// 응답 메시지 수신
	if ((n = msgrcv(msqid, (char *)&msg, sizeof(msg), MSG_REPLY, 0)) < 0)  {
		perror("msgrcv");
		exit(1);
	}
	printf("Received reply: %s\n", msg.data);
	
	return 0;
}

