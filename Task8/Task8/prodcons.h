#define	MY_ID			19 // 프로그램 식별자
#define	SHM_KEY			(0x9000 + MY_ID) // 공유 메모리 세그먼트를 위한 키
// 공유 메모리 생성 모드 (읽기, 쓰기, 생성)
#define	SHM_MODE		(SHM_R | SHM_W | IPC_CREAT)

#define	EMPTY_SEM_KEY	(0x5000 + MY_ID) // 버퍼에서 빈 슬롯을 나타내는 세마포어 키
#define	FULL_SEM_KEY	(0x6000 + MY_ID) // 버퍼에서 가득 찬 슬롯을 나타내는 세마포어 키
#define	MUTEX_SEM_KEY	(0x7000 + MY_ID) // 공유 버퍼에 접근하는 데 사용되는 뮤텍스 세마포어 키

#define	NLOOPS			20 // 반복 횟수

#define	MAX_BUF			2 // 버퍼 최대 크기

#define	SEM_KEY	(0x7000 + MY_ID)

typedef struct  { // 아이템을 나타내는 구조체
	int		data;
} ItemType;

typedef struct  { // 제한된 버퍼를 나타내는 구조체
	ItemType	buf[MAX_BUF];
	int			in;
	int			out;
	int			counter;
} BoundedBufferType;

#define	SHM_SIZE	sizeof(BoundedBufferType) // 공유 메모리 크기
