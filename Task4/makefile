# 컴파일러 및 컴파일러 플래그 설정
C = gcc
CFLAGS =
LDFLAGS =

.SUFFIXES : .c .o
.c.o :
	$(CC) -c $(CFLAGS) $<

# 빌드할 프로그램 목록
ALL = stat umask chmod myln mysln myrm mymv mymd myrd myls mylsr mycd mypwd

all: $(ALL)

# 컴파일 및 링크 규칙
stat: stat.o
	$(CC) -o $@ $< $(LDFLAGS)

umask: umask.o
	$(CC) -o $@ $< $(LDFLAGS)

chmod: chmod.o
	$(CC) -o $@ $< $(LDFLAGS)

myln: myln.o
	$(CC) -o $@ $< $(LDFLAGS)

mysln: mysln.o
	$(CC) -o $@ $< $(LDFLAGS)

myrm: myrm.o
	$(CC) -o $@ $< $(LDFLAGS)

mymv: mymv.o
	$(CC) -o $@ $< $(LDFLAGS)

mymd: mymd.o
	$(CC) -o $@ $< $(LDFLAGS)

myrd: myrd.o
	$(CC) -o $@ $< $(LDFLAGS)

myls: myls.o
	$(CC) -o $@ $< $(LDFLAGS)

mylsr: mylsr.o
	$(CC) -o $@ $< $(LDFLAGS)

mycd: mycd.o
	$(CC) -o $@ $< $(LDFLAGS)

mypwd: mypwd.o
	$(CC) -o $@ $< $(LDFLAGS)
# 'clean' 타겟 설정
clean :
	rm -rf *.o $(ALL)
