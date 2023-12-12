// 사용자 ID를 나타내는 매크로
#define MY_ID 0

#define SERV_TCP_PORT (7000 + MY_ID) // TCP 서버 포트 번호 (사용자 ID를 고려하여 설정)
#define SERV_UDP_PORT (8000 + MY_ID) // UDP 서버 포트 번호 (사용자 ID를 고려하여 설정)
#define UNIX_STR_PATH "./.unix-str" // UNIX 도메인 소켓 연결 지향 서버 경로
#define UNIX_DG_PATH "./.unix-dg" // UNIX 도메인 소켓 비연결 지향 서버 경로

#define	MSG_REQUEST		1
#define	MSG_REPLY		2

// 메시지 타입을 정의하는 구조체
typedef struct {
    int type;          // 메시지 타입 (요청: MSG_REQUEST, 응답: MSG_REPLY)
    char data[128];     // 메시지 데이터 (최대 128바이트)
} MsgType;

