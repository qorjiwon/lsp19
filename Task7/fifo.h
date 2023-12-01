#define	MAX_FIFO_NAME	32

#define	SERV_FIFO	"./.fifo"

// 통신을 위한 메시지 타입을 정의하는 구조체
typedef struct  {
	char	returnFifo[MAX_FIFO_NAME]; // 클라이언트로부터 서버로의 응답을 위한 FIFO 이름
	char	data[128]; // 실제 데이터를 저장하는 배열
}
	MsgType;
