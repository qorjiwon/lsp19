#include <stdio.h>
#include <pthread.h>

#define N 100

int sum1 = 0;
int sum2 = 0;

void *calculateSum(void *arg) {
    int start = *((int *)arg);
    int end = *((int *)arg + 1);
    int *result = (int *)malloc(sizeof(int));
    *result = 0;

    for (int i = start; i <= end; i++) {
        *result += i;
    }

    return (void *)result;
}

int main() {
    int range1[2] = {1, N / 2};
    int range2[2] = {N / 2 + 1, N};

    pthread_t thread1, thread2;
    void *result1, *result2;

    pthread_create(&thread1, NULL, calculateSum, (void *)&range1);
    pthread_create(&thread2, NULL, calculateSum, (void *)&range2);

    pthread_join(thread1, &result1);
    pthread_join(thread2, &result2);

    sum1 = *((int *)result1);
    sum2 = *((int *)result2);

    printf("Sum from thread 1: %d\n", sum1);
    printf("Sum from thread 2: %d\n", sum2);
    printf("Total Sum: %d\n", sum1 + sum2);

    return 0;
}
