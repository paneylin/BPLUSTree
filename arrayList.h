#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifndef ARRAYLIST_H
#define ARRAYLIST_H

typedef struct  ArrayList{
    void **data;
    int dataSize;
    int currentSize;
    void * (*getKeyFunc)(void *);
    int (*compareFunc)(void *, void *);
}ArrayList;

const int ARRAYLIST_DEFAULT_SIZE = 10;

//创建ArrayList，默认比较为地址比较
ArrayList * createArrayListAList(void * (*getKeyFunc)(void *data) , int (*compareFunc)(void * key1, void *key2));
//插入节点
void insertElementAList(void *data , ArrayList * list);
//获取节点数据，根据key返回节点数据
void * getElementAList(void * key , ArrayList * list);
//获取节点数据,index从0开始，如果大于list结点数量，返回null
void * getElementByIndexAList(int index , ArrayList * list);
//删除节点
void deleteElementAList(void * Key , ArrayList * list);
//删除节点根据index
void deleteElementByIndexAList(int index , ArrayList * list);
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

