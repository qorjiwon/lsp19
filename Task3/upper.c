#include <stdio.h>

main(int argc, char *argv[]){
	int ch; 	// 문자를 저장할 변수
	FILE *src;	// 입력 파일을 가리킬 파일 포인터

	// 명령줄 인수가 2개가 아닌 경우, 사용법을 출력하고 프로그램 종료
	if (argc != 2) {
		printf("Usage: %s source destination\n", argv[0]);
		exit(1);
	}

	// 입력 파일 열기
	if ((src = fopen(argv[1], "rt")) == NULL){
		perror("fopen"); // fopen 함수가 실패하면 오류 메시지 출력
		exit(1); // 프로그램 종료
	}

	while ((ch = fgetc(src)) != EOF) { // 파일의 끝까지 반복
		if (ch >= 'a' && ch <= 'z') {
			ch = ch - 32; // 소문자를 대문자로 변경
		}
	putchar(ch); // 문자 출력
	}
}
// 아래와 같은 명령어를 통해 해당 파일을 테스트 할 수 있음.
// ./upper test.txt
// test.txt 내용을 소문자->대문자로 변환하여 출력.
// test.txt 내용은 다음과 같음
// Congratulations, Sanghoon
// Have a rewarding and happy last semester
// I hope the future will be full of happiness
// 명령어 실행 결과는 다음과 같음
// CONGRATULATIONS, SANGHOON
// HAVE A REWARDING AND HAPPY LAST SEMESTER
// I HOPE THE FUTURE WILL BE FULL OF HAPPINESS
