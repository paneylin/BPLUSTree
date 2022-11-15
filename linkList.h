
#ifndef LINK_LIST_H
#define LINK_LIST_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct LinkListNode{
    void * key;
    void * data;
    struct LinkListNode * pre;
    struct LinkListNode * next;
}LinkListNode;

typedef struct  LinkList{
    LinkListNode * head , * tail;
    void * (*getKeyFunc)(void *);
    int (*compareFunc)(void *, void *);
    void (*freeKeyFunc)(void *);
    void (*freeDataFunc)(void *);
}LinkList;

//创建linklist，如果key固定，默认data就是key
LinkList * createLinkListLList(void * (*getKeyFunc)(void *data) , int (*compareFunc)(void * key1, void *key2));
//设置释放key函数，不设置不释放
void setFreeKeyFuncLList(void (*freeKeyFunc)(void * key) , LinkList * list);
//设置释放data函数，不设置不释放
void setFreeDataFuncLList(void (*freeDataFunc)(void * data) , LinkList * list);
//插入节点
void insertElementLList(void *data , LinkList * list);
//获取节点数据，根据key返回节点数据
void * getElementLList(void * key , LinkList * list);
//获取节点数据,index从0开始，如果大于list结点数量，返回null
void * getElementByIndexLList(int index , LinkList * list);
//删除节点
void deleteElementLList(void * Key , LinkList * list);
//判断是否为空
int isEmptyLList(LinkList * list);
//释放linklist内存
void destroyLList(LinkList * list);

void showLList(LinkList * list , void (*showFunc)(void *key , void *data));

#include "./linkList.c"

#endif