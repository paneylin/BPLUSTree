#ifndef FIBONACCIHEAP_H
#define FIBONACCIHEAP_H

#include"./arrayList.h"
#include"./CircleQueue.h"
#include<math.h>

typedef struct HeapNodeFHeap
{
    void *key;
    void *data;
    int degree;
    struct HeapNodeFHeap *parent;
    struct HeapNodeFHeap *child;
    struct HeapNodeFHeap *right;
    struct HeapNodeFHeap *left;
    struct HeapNodeFHeap *topChild;
}HeapNodeFHeap;

typedef struct FHeap
{
    HeapNodeFHeap *topNode;
    ArrayList ** degreeList;
    int dgreeSize;
    int maxDegree;
    int heapSize;
    void * (*getKeyFunc)(void *);
    int (*compareFunc)(void * , void *);
    void (*freeDataFunc)(void *);
    void (*freeKeyFunc)(void *);
}FHeap;
//创建堆函数，compafunc如果第一个大于第二个参数则返回正数则是最大堆，第二个参数大于第一个参数时候返回正数则是最小堆
FHeap *createFHeap(int (*compareFunc)(void * key1, void *key2) , void *(*getKeyFunc)(void * data));
//设置堆的数据释放函数
void setFreeDataFuncFHeap(void (*freeDataFunc)(void * data) , FHeap * heap);
//设置释放key的函数
void setFreeKeyFuncFHeap(void (*freeKeyFunc)(void * key) , FHeap * heap);
//插入批量数据
ArrayList *insertElementsFHeap(void **data , int dataSize , FHeap *heapInfo);
//插入单个数据
HeapNodeFHeap *insertElementFHeap(void *data , FHeap * heapInfo);
//获取堆顶数据并移除该数据
void *popElementHeapFHeap(FHeap * heapInfo);
//获取堆顶数据不移除数据
void * getElementFHeap(FHeap *heapInfo);
//返回当前heap元素数量
int getHeapSizeFHeap(FHeap * heap);
//返回当前heap是否为空
int isEmptyFHeap(FHeap * heap);
//合并heap1跟heap2，返回合并后的heap，同时释放heap1与heap2
FHeap * combindHeapsFHeap(FHeap *heap1 , FHeap *heap2);
//更新key1值为key2值
 void updateKeyFHeap(void *key1 , void *key2 , FHeap *heap);
//销毁堆栈并释放内存
 void destoryFHeap(FHeap * heap);
//更新node对应的key值大小，并返回该key对应的新节点node
 HeapNodeFHeap *updateKeyWithNodeFHeap(void * key , HeapNodeFHeap * node , FHeap * heap);
//查看key是否在FHeap当中
 int isInFHeap(void * key , FHeap * heap);

 const int DEFAULT_DEGREE_SIZE = 100;

#include "./fibonacciHeap.c"
#endif