#include <stdio.h>
#include <sys/types.h>
#include <sys/mman.h>
#include <fcntl.h>
#include "synclib.h"

#define	NLOOPS	10
#define	SIZE	sizeof(long)


int update(long *ptr)
{
	return ((*ptr)++);
}


int main()
{
	int		fd, i, counter;
	pid_t	pid;
	caddr_t	area;

	// /dev/zero 파일 열기 (무한한 0으로 구성된 가상 메모리 매핑을 생성)
	if ((fd = open("/dev/zero", O_RDWR)) < 0)  {
		perror("open");
		exit(1);
	}

	// 메모리 매핑 생성 (읽기 및 쓰기 권한, 공유 매핑, 파일 지정 없음)
	if ((area = mmap(0, SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0))
		== (caddr_t) -1)  {
		perror("mmap");
		exit(1);
	}
	close(fd); // 파일 디스크립터 닫기 (더 이상 필요하지 않음)

	TELL_WAIT();
	if ((pid = fork()) < 0)  {
		perror("fork");
		exit(1);
	}
	else if (pid > 0)  {
		for (i = 0 ; i < NLOOPS ; i += 2)  {
			// 공유 변수 업데이트 및 확인
			if ((counter = update((long *) area)) != i)  {
				fprintf(stderr, "Counter mismatch\n");
				exit(1);
			}
			printf("Parent: counter=%d\n", counter);
			TELL_CHILD();
			WAIT_CHILD();
		}
	}
	else  {
		for (i = 1 ; i < NLOOPS ; i += 2)  {
			WAIT_PARENT();

			// 공유 변수 업데이트 및 확인
			if ((counter = update((long *) area)) != i)  {
				fprintf(stderr, "Counter mismatch\n");
				exit(1);
			}
			printf("Child : counter=%d\n", counter);
			TELL_PARENT();
		}
	}
	return 0;
}
