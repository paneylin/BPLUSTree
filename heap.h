
#ifndef HEAP_H
#define HEAP_H

#include"./CircleQueue.h"
#include<math.h>

typedef struct HeapInfo
{
    void **heapData;
    int heapSize;
    int dataSize;
    int (*compareFunc)(void *,void *);
}HeapInfo;

//创建堆函数，compafunc如果第一个大于第二个参数则返回正数则是最大堆，第二个参数大于第一个参数时候返回正数则是最小堆
HeapInfo *createHeapInfoHeap(int basicHeapSize ,int (*compareFunc)(void * , void *));
//插入批量数据
void insertElementsHeap(void **data , int dataSize , HeapInfo *heapInfo);
//插入单个数据
void insertElementHeap(void *data , HeapInfo * heapInfo);
//展示堆信息
void showHeapInfo(HeapInfo * heapInfo , void (*showFunc)(void *));
//获取堆顶数据并移除该数据
void *popElementHeap(HeapInfo * heapInfo);
//获取堆顶数据不移除数据
void * getElementHeap(HeapInfo *heapInfo);
//返回当前heap元素数量
int getHeapSizeHeap(HeapInfo * heap);
//返回当前heap是否为空
int isEmptyHeap(HeapInfo * heap);

#include"./heap.c"

#endif