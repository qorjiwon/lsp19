#include <stdio.h>
#include <string.h>
#include "record.h"

main(int argc, char *argv[])
{
	FILE	*src, *dst; // 소스 파일, 대상 파일
	Record	rec; // 사용자 정의 레코드 타입

	if (argc != 3)  { // 실행 파일, 소스 파일, 대상 파일
		fprintf(stderr, "Usage: %s source dest\n", argv[0]);
		exit(1);
	}

	if ((src = fopen(argv[1], "rt")) == NULL)  {
		perror("fopen");
		exit(1);
	}
	if ((dst = fopen(argv[2], "wb")) == NULL)  {
		perror("fopen");
		exit(1);
	}

	while (fgets(rec.stud, SMAX, src))  { // 학생 이름을 파일에서 읽어서 rec.stud에 저장
		*strchr(rec.stud, '\n') = '\0'; // 줄 바꿈 문자를 널 문자로 대체하여 문자열을 종료
		fgets(rec.num, NMAX, src); // 학번을 파일에서 읽어서 rec.num에 저장
		*strchr(rec.num, '\n') = '\0'; // 학번 문자열을 종료
		fgets(rec.dept, DMAX, src); // 학과를 파일에서 읽어서 rec.dept에 저장
		*strchr(rec.dept, '\n') = '\0'; // 학과 문자열을 종료

		fwrite(&rec, sizeof(rec), 1, dst); // 읽어온 레코드를 대상 파일에 이진 형식으로 쓰기
	}

	fclose(src);
	fclose(dst);
}
