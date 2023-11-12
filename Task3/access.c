#include <stdio.h>
#include <unistd.h>
#include "record.h" // Record 구조체 정의가 들어있음

void
print_record(Record *rp)
{
	puts(rp->stud); // Record 구조체의 학생 이름 출력
	puts(rp->num); // Record 구조체의 학번 출력
	puts(rp->dept); // Record 구조체의 학과 출력
}

main(int argc, char *argv[])
{
	FILE	*fp; // 파일 포인터
	Record	rec; // Record 구조체

	if (argc != 2)  { // 실행 파일, 소스 파일
		fprintf(stderr, "Usage: %s file\n", argv[0]); // 프로그램 사용법 출력
		exit(1);
	}

	if ((fp = fopen(argv[1], "r+b")) == NULL)  {
		perror("fopen"); // 파일 열기 오류 발생 시 에러 메시지 출력
		exit(1);
	}

	printf("-----Ordered Record List-----\n");

	while (fread(&rec, sizeof(rec), 1, fp))  {
		print_record(&rec); // 파일에서 Record를 읽어서 출력 
	}
	rewind(fp); // 파일 포인터를 파일의 처음으로 다시 설정
	getchar(); // 사용자 입력 대기

	printf("-----Shuffled Record List-----\n");

	// 파일에서 특정 위치의 Record를 읽고 출력
	fseek(fp, sizeof(rec)*2L, SEEK_SET);
	fread(&rec, sizeof(rec), 1, fp);
	print_record(&rec);

	fseek(fp, sizeof(rec)*5L, SEEK_SET);
	fread(&rec, sizeof(rec), 1, fp);
	print_record(&rec);

	fseek(fp, sizeof(rec)*1L, SEEK_SET);
	fread(&rec, sizeof(rec), 1, fp);
	print_record(&rec);

	fseek(fp, sizeof(rec)*3L, SEEK_SET);
	fread(&rec, sizeof(rec), 1, fp);
	print_record(&rec);

	fseek(fp, sizeof(rec)*0L, SEEK_SET);
	fread(&rec, sizeof(rec), 1, fp);
	print_record(&rec);

	fseek(fp, sizeof(rec)*4L, SEEK_SET);
	fread(&rec, sizeof(rec), 1, fp);
	print_record(&rec);
	getchar();

	printf("-----Updated Record List-----\n");

	// 파일에서 특정 위치의 Record를 읽고 학과 정보를 업데이트
	fseek(fp, sizeof(rec)*2L, SEEK_SET);
	fread(&rec, sizeof(rec), 1, fp);
	strcpy(rec.dept, "Movie");
	fseek(fp, sizeof(rec)*2L, SEEK_SET);
	fwrite(&rec, sizeof(rec), 1, fp);

	fseek(fp, sizeof(rec)*5L, SEEK_SET);
	fread(&rec, sizeof(rec), 1, fp);
	strcpy(rec.dept, "Baseball");
	fseek(fp, sizeof(rec)*5L, SEEK_SET);
	fwrite(&rec, sizeof(rec), 1, fp);

	rewind(fp);
	while (fread(&rec, sizeof(rec), 1, fp))  {
		print_record(&rec); // 업데이트된 Record 출력
	}

	fclose(fp); // 파일 닫기
}
