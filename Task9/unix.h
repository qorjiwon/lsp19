// UNIX 도메인 소켓 통신을 위한 파일 경로 정의
#define UNIX_STR_PATH "./.unix-str"  // 스트림 소켓 파일 경로
#define UNIX_DG_PATH "./.unix-dg"   // 데이터그램 소켓 파일 경로

// 메시지 타입 상수 정의
#define MSG_REQUEST 1  // 요청 메시지 타입
#define MSG_REPLY 2    // 응답 메시지 타입

// 메시지 구조체 정의
typedef struct {
    int type;          // 메시지의 타입을 저장하는 멤버
    char data[128];    // 데이터를 저장하는 멤버, 크기는 128바이트
} MsgType;
