#ifndef MEMORY_H
#define MEMORY_H

#include "./leftRightTree.h"

//分配size内存，并返回内存地址
void* mallocMemory(int size);
//调整内存大小为size，并返回内存地址
void *rellocMemory(void *memory , int size);
//释放内存
void freeMemory(void *memory);

#include "./memory.c"

#endif