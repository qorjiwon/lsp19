taks_exe : test1.o test2.o main.o # target : taks_exe, prerequisites : test1.o, test2.o, main.o -> taks_exe을 만들기 위해 세 개의 파일(prerequisites) 필요
	gcc -o taks_exe test1.o test2.o main.o # taks_exe를 만들기 위해 실행되는 command
test1.o : test1.c # target : test1.o, prerequisite : test1.c
	gcc -c -o test1.o test1.c # test1.o가 없다면 만들기 위해 실행되는 command
test2.o : test2.c # target : test2.o, prerequisite : test2.c
	gcc -c -o test2.o test2.c # test2.o가 없다면 만들기 위해 실행되는 command
main.o : main.c # target : main.o, prerequisite : main.c
	gcc -c -o main.o main.c # main.o가 없다면 만들기 위해 실행되는 command
clean : # 불필요한 파일들을 정리하기 위한 명령어
	rm *.o taks_exe # .o 파일을 모두 제거, taks_exe 파일 제거
