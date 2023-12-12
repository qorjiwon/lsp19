#include <stdio.h>

main()
{
    // 정수 변수 a를 0x12345678로 초기화
    int a = 0x12345678;

    // 문자 포인터 p를 정수 변수 a의 주소로 초기화
    char *p = (char *)&a;

#if 1
    // 주소와 해당 주소에 있는 값을 출력하고 포인터를 증가시킴
    printf("Address %p: %#x\n", p, *p); p++;
    printf("Address %p: %#x\n", p, *p); p++;
    printf("Address %p: %#x\n", p, *p); p++;
    printf("Address %p: %#x\n", p, *p); p++;
#else
    // 주소와 해당 주소에 있는 값을 출력하고 포인터를 증가시킴 (단일 행으로 압축)
    printf("Address %p: %#x\n", p, *p++);
    printf("Address %p: %#x\n", p, *p++);
    printf("Address %p: %#x\n", p, *p++);
    printf("Address %p: %#x\n", p, *p++);
#endif
}
