#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#ifndef CircleQueue_H
#define CircleQueue_H

const int IINIT_SIZE_CIRCLE_QUEUE = 10;

typedef struct CircleQueue{
    void **data;
    int size;
    int head , tail;
    void (*freeDataFunc)(void *data);
}CircleQueue;

//创建循环队列，并返回循环队列结构
CircleQueue *createCircleQueue();
//获取循环队列第一个元素
void *getElementsFromCircleQueue(CircleQueue *queue);
//将data放入循环队列
void pushCircleQueue(void *data , CircleQueue *queue);
//获取循环队列第一个元素并从循环队列中删除该元素
void *popCircleQueue(CircleQueue *queue);
//释放循环队列所占内存,队列中的元素不会被释放
void destroyCircleQueue(CircleQueue *queue);
//判断循环队列中的元素是否为空
int isEmptyCircleQueue(CircleQueue *queue);
//展示循环队列中元素
void showCirCleQueue(CircleQueue *circleQueue , void (*showFunc)(void *data));
//清除循环队列当中的元素
void clearCircleQueue(CircleQueue *queue);
//设置释放数据函数，销毁队列时候释放对象内存
void setFreeDataFuncCircleQueue(void (*freeDataFunc)(void *data) , CircleQueue *queue);
#include "./CircleQueue.c"
#endif
