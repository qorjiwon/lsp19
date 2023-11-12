C = gcc
CFLAGS =
LDFLAGS =

# .c 파일을 .o 파일로 컴파일하기 위한 규칙
.SUFFIXES : .c .o
.c.o :
	$(CC) -c $(CFLAGS) $<

#빌드할 모든 실행 파일 정의
ALL = mycp hole list tcp1 tcp2 mycp2 split merge conv access tab

# 모든 실행 파일 빌드
all: $(ALL)

# 각 실행 파일을 빌드하는 규칙
# $@ : 대상 파일 이름,	$< : 종속 파일(소스 파일) 이름
mycp: mycp.o
	$(CC) -o $@ $< $(LDFLAGS)

hole: hole.o
	$(CC) -o $@ $< $(LDFLAGS)

list: list.o
	$(CC) -o $@ $< $(LDFLAGS)

tcp1: tcp1.o
	$(CC) -o $@ $< $(LDFLAGS)

tcp2: tcp2.o
	$(CC) -o $@ $< $(LDFLAGS)

mycp2: mycp2.o
	$(CC) -o $@ $< $(LDFLAGS)

split: split.o
	$(CC) -o $@ $< $(LDFLAGS)

merge: merge.o
	$(CC) -o $@ $< $(LDFLAGS)

conv: conv.o
	$(CC) -o $@ $< $(LDFLAGS)

access: access.o
	$(CC) -o $@ $< $(LDFLAGS)

tab: tab.o
	$(CC) -o $@ $< $(LDFLAGS)

# 빌드한 실행 파일 및 .o 파일 정리
clean :
	rm -rf *.o $(ALL) file.hole
