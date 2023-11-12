# 컴파일러, 컴파일 옵션, 링크 옵션 정의
CC = gcc
CFLAGS =
LDFLAGS = -lpthread

# .c 파일에서 .o 파일을 생성하기 위한 규칙
.SUFFIXES : .c .o
.c.o :
	$(CC) -c $(CFLAGS) $<

# 모든 실행 파일의 이름을 정의
ALL = fork1 fork exit wait race worace env exec system thread cmd_i cmd_p cmd_t

# 기본적으로 모든 실행 파일 빌드
all: $(ALL)

# 각 실행 파일에 대한 빌드 규칙
fork1: fork1.o
	$(CC) -o $@ $< $(LDFLAGS)

fork: fork.o
	$(CC) -o $@ $< $(LDFLAGS)

exit: exit.o
	$(CC) -o $@ $< $(LDFLAGS)

wait: wait.o
	$(CC) -o $@ $< $(LDFLAGS)

race: race.o
	$(CC) -o $@ $< $(LDFLAGS)

worace: worace.o
	$(CC) -o $@ $< $(LDFLAGS)

env: env.o
	$(CC) -o $@ $< $(LDFLAGS)

exec: exec.o
	$(CC) -o $@ $< $(LDFLAGS)

system: system.o
	$(CC) -o $@ $< $(LDFLAGS)

thread: thread.o
	$(CC) -o $@ $< $(LDFLAGS)

cmd_i: cmd_i.o
	$(CC) -o $@ $< $(LDFLAGS)

cmd_p: cmd_p.o
	$(CC) -o $@ $< $(LDFLAGS)

cmd_t: cmd_t.o
	$(CC) -o $@ $< $(LDFLAGS)

# .o 파일 및 실행 파일 정리
clean :
	rm -rf *.o $(ALL)

