// 사용자 ID 및 메시지 키 정의
#define	MY_ID			0
#define	MSG_KEY			(0x8000 + MY_ID)

// 메시지 타입 정의
#define	MSG_REQUEST		1
#define	MSG_REPLY		2

// 메시지 큐를 위한 구조체 정의
typedef struct  {
	long	type;		// 메시지의 유형을 나타내는 필드 (메시지 큐에서는 반드시 long 형이어야 함)
	char	data[128];	// 실제 데이터를 저장하는 배열
}
	MsgType;
