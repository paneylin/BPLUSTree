
#ifndef HEAP_H
#define HEAP_H

#include"./CircleQueue.h"
#include"./memory.h"
#include"./arrayList.h"
#include<math.h>

typedef struct NodeHeapInfo
{
    void * data;
    int index;
}NodeHeapInfo;

typedef struct HeapInfo
{
    NodeHeapInfo **heapData;
    int heapSize;
    int dataSize;
    int (*compareFunc)(NodeHeapInfo *,NodeHeapInfo *);
    void (*freeDataFunc)(void *);
}HeapInfo;


//创建堆函数，compafunc如果第一个大于第二个参数则返回正数则是最大堆，第二个参数大于第一个参数时候返回正数则是最小堆
HeapInfo *createHeapInfoHeap(int basicHeapSize ,int (*compareFunc)(NodeHeapInfo * data1, NodeHeapInfo *data2));
//插入批量数据
ArrayList *insertElementsHeap(void **data , int dataSize , HeapInfo *heapInfo);
//设置释放heapdata函数，释放heap的时候同步释放data
void setFreeDataFuncHeap(void (*freeDataFunc)(void *),HeapInfo *heapInfo);
//插入单个数据
NodeHeapInfo *insertElementHeap(void *data , HeapInfo * heapInfo);
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
//节点优先级下降操作
void increaseDataHeap(NodeHeapInfo * nodeHeapInfo , HeapInfo * heapInfo);
//节点优先级上升操作
void decreseDataHeap(NodeHeapInfo * nodeHeapInfo , HeapInfo * heapInfo);

#include"./heap.c"

#endif