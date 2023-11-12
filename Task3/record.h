#define	SMAX	20 // 학생 이름 최대 길이
#define	NMAX	10 // 학번 최대 길이
#define	DMAX	15 // 학과 정보 최대 길이

typedef struct {
	char	stud[SMAX];	// 학생 이름 문자열 배열
	char	num[NMAX];	// 학번 문자열 배열
	char	dept[DMAX];	// 학과 문자열 배열
} Record;
