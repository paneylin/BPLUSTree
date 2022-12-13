#include "./memory.h"

int main(){
    int * p = mallocMemory(sizeof(int));
    freeMemory(p);
    printf("freeMemory(p) success\n");
    freeMemory(p);
}

