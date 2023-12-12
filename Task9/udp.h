// 사용자의 ID를 나타내는 상수 정의
#define MY_ID 0

// 서버 UDP 포트를 정의, 사용자 ID를 포함하여 포트를 동적으로 설정
#define SERV_UDP_PORT (8000 + MY_ID)

// 서버의 호스트 주소를 문자열로 정의
#define SERV_HOST_ADDR "127.0.0.1"

// 메시지 타입을 나타내는 상수 정의
#define MSG_REQUEST 1
#define MSG_REPLY 2

// MsgType이라는 구조체 정의
typedef struct  {
    int type;         // 메시지의 타입을 저장하는 멤버
    char data[128];   // 데이터를 저장하는 멤버, 크기는 128바이트
} MsgType;
