#include <stdio.h>

enum { FALSE, TRUE};

void
conv(char *fname)
{
	FILE	*src, *dst; // 입력 파일, 출력 파일
	char	fdest[40];  // 출력 파일 이름 저장
	int		ch, first; // 현재 문자, 해당 줄의 처음인지 여부

	if ((src = fopen(fname, "rt")) == NULL)  {
		perror("fopen");
		return;
	}

	// 출력 파일 이름 생성 (원본 파일 이름 + ".t")
	strcpy(fdest, fname);
	strcat(fdest, ".t");

	if ((dst = fopen(fdest, "wt")) == NULL)  {
		perror("fopen");
		return;
	}

	first = TRUE; // 초기화
	while ((ch = fgetc(src)) != EOF)  {
		if (first && ch == '\t')  {
			// 첫 번째 문자가 탭 문자인 경우 스페이스 4개로 대체
			fputc(' ', dst);
			fputc(' ', dst);
			fputc(' ', dst);
			fputc(' ', dst);
		}
		else  {
			fputc(ch, dst); // 다른 경우, 현재 문자를 출력 파일에 쓰기.
			if (first)
				first = FALSE; // 줄의 처음이 아니도록 설정
			if (ch == '\n')
				first = TRUE; // 줄 바꿈 문자를 만나면 다음 줄의 처음으로 설정
		}
	}

	fclose(src);
	fclose(dst);
}

main(int argc, char *argv[])
{
	while (--argc)  {
		conv(argv[argc]); // 명령행에서 전달된 각 파일에 대해 'conv' 함수 호출
	}
}
