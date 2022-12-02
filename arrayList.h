#ifndef ARRAYLIST_H
#define ARRAYLIST_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct  ArrayList{
    void **data;
    int dataSize;
    int currentSize;
    int (*compareFunc)(void *, void *);
    void (*freeDataFunc)(void *);
}ArrayList;

const int ARRAYLIST_DEFAULT_SIZE = 10;

//创建ArrayList，默认比较为地址比较
ArrayList * createArrayListAList(int (*compareFunc)(void * data1, void *data2));
//设置释放数据函数，如果设置了，释放list的时候释放数据内存
void setFreeDataFuncAList(void (*freeDataFunc)(void * data), ArrayList * list);
//插入节点
void insertElementAList(void *data , ArrayList * list);
//批量添加元素到ArrayList
void insertElementsAList(void **datas , int length , ArrayList * list);
//添加一个ArrayList元素到另一个ArrayList,list1加入list2
void insertArrayListAList(ArrayList *list1 , ArrayList *list2);
//获取节点数据，根据key返回节点数据,默认地址比较
void * getElementAList(void *data , ArrayList * list);
//获取节点数据,index从0开始，如果大于list结点数量，返回null
void * getElementByIndexAList(int index , ArrayList * list);
//获取list第一个元素数据
void * getFirstElementAList(ArrayList * list);
//获取list最后一个元素数据
void * getLastElementAList(ArrayList * list);
//删除节点,并返回删除节点的数据
void* deleteElementAList(void * Key , ArrayList * list);
//删除节点根据index，并返回删除节点的数据
void* deleteElementByIndexAList(int index , ArrayList * list);
//释删除list最后一个元素并返回数据
void * deleteLastElementAList(ArrayList * list);
//判断是否为空
int isEmptyAList(ArrayList * list);
//释放linklist内存
void destroyAList(ArrayList * list);
//获取list数据大小
int getSizeAList(ArrayList * list);
//显示列表所有数据
void showAList(ArrayList * list , void (*showFunc)(void *data) );

#include "./arrayList.c"
#endif

