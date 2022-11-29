#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<math.h>

int main(){
    int mun = 0;
    scanf("%d", &mun);
    int (*a)[2];
    a = (int *)malloc(sizeof(int) * 2 * mun);
    printf("%d ", a[0]);
    printf("%d ", a[0] + 1);
    printf("%d ", a);
    printf("%d ", a + 1);
    printf("%d ", &a[mun - 1][1]);
}