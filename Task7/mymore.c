#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main() 
{
	int fd[2]; // 파일 디스크립터를 저장할 배
	pid_t child; // 자식 프로세스의 PID를 저장할 변수

	if(pipe(fd) == -1)  { // pipe 생성
		perror("pipe");
		exit(1);
	}
 
	child = fork(); // fork를 사용
	if(child == (pid_t)(-1))  {
        	perror("fork");
		exit(1);
	}
	else if(child == (pid_t)0) { // 자식 프로세스의 동작
		// 표준 출력을 파이프의 쓰기 엔드로 재지정
        	close(1); // 표준 출력을 닫음
		close(fd[0]); // 파이프의 읽기 엔드를 닫
        	
        	if(dup(fd[1]) == -1)  {
        		perror("dup");
			exit(1);
		}

		// ls -l 명령을 실행해서 결과를 파이프로 출력
        	if((execlp("ls", "ls", "-l", NULL)) == -1)  {
        		perror("execlp");
			exit(1);
		}
	} 
	else { // 부모 프로세스의 동작
        	close(0);
		close(fd[1]);
        	
        	if(dup2(fd[0], 0) == -1)  {
        		perror("dup");
			exit(1);
		}

		// more 명령을 실행해서 자식 프로세스의 출력을 읽어 화면에 표시시
        	if((execlp("more", "more", NULL)) == -1)  {
        		perror("execlp");
			exit(1);
		}
	}
 
	return 0;
}
