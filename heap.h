
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
    void (*freeDataFunc)(void *);
}HeapInfo;

//创建堆函数，compafunc如果第一个大于第二个参数则返回正数则是最大堆，第二个参数大于第一个参数时候返回正数则是最小堆
HeapInfo *createHeapInfoHeap(int basicHeapSize ,int (*compareFunc)(void * data1, void *data2));
//插入批量数据
void insertElementsHeap(void **data , int dataSize , HeapInfo *heapInfo);
//设置释放heapdata函数，释放heap的时候同步释放data
void setFreeDataFuncHeap(void (*freeDataFunc)(void *),HeapInfo *heapInfo);
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
//释放heap的内存并销毁
void destroyHeapInfoHeap(HeapInfo * heapInfo);

#include"./heap.c"

#endif