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
}LinkList;

//创建linklist，如果key固定，可以传入getKeyFunc,freeKeyFunc，如果不固定，传入NULL
LinkList * createLinkListLList(void * (*getKeyFunc)(void *) , int (*compareFunc)(void * , void *) , void (*freeKeyFunc)(void *));
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

void showLList(LinkList * list , void (*showFunc)(void *));

int compare_default_LList(void * a, void * b){
    return *(int *)a  - *(int *)b;
}

void * get_key_default_LList(void * data){
    return data;
}

void free_key_default_LList(void * key){
    return;
}

LinkList * createLinkListLList(void * (*getKeyFunc)(void *) , int (*compareFunc)(void * , void *) , void (*freeKeyFunc)(void *)){
    LinkList * list = (LinkList *)malloc(sizeof(LinkList));
    list->head = list->tail = NULL;
    list->getKeyFunc = getKeyFunc;
    if(getKeyFunc == NULL){
        list->getKeyFunc = get_key_default_LList;
    }
    list->compareFunc = compareFunc;
    if(compareFunc == NULL){
        list->compareFunc = compare_default_LList;
    }
    list->freeKeyFunc = freeKeyFunc;
    if(freeKeyFunc == NULL){
        list->freeKeyFunc = free_key_default_LList;
    }
    return list;
}

void * create_key_LList(void * data ,LinkList * list){
    return list->getKeyFunc(data);
}

void insertElementLList(void *data , LinkList * list){
    LinkListNode * node = (LinkListNode *)malloc(sizeof(LinkListNode));
    node->data = data;
    node->key = create_key_LList(data , list);
    node->next = NULL;
    node->pre = NULL;
    if(list->head == NULL){
        list->head = list->tail = node;
    }else{
        list->tail->next = node;
        node->pre = list->tail;
        list->tail = node;
    }
}

void * getElementLList(void * key , LinkList * list){
    LinkListNode * node = list->head;
    while(node != NULL){
        if(list->compareFunc(node->key , key) == 0){
            return node->data;
        }
        node = node->next;
    }
    return NULL;
}

void * getElementByIndexLList(int index , LinkList * list){
    LinkListNode * node = list->head;
    int i = 0;
    while(node != NULL){
        if(i == index){
            return node->data;
        }
        i++;
        node = node->next;
    }
    return NULL;
}

void deleteElementLList(void * key , LinkList * list){
    LinkListNode * node = list->head;
    while(node != NULL){
        if(list->compareFunc(node->key , key) == 0){
            if(node->pre != NULL){
                node->pre->next = node->next;
            }else{
                list->head = node->next;
            }
            if(node->next != NULL){
                node->next->pre = node->pre;
            }else{
                list->tail = node->pre;
            }
            free(node);
            break;
        }
        node = node->next;
    }
}

int isEmptyLList(LinkList * list){
    return list->head == NULL;
}

void destroyLList(LinkList * list){
    LinkListNode * node = list->head;
    while(node != NULL){
        LinkListNode * temp = node;
        node = node->next;
        list->freeKeyFunc(temp->key);
        free(temp);
    }
    free(list);
}

void showLList(LinkList * list , void (*showFunc)(void *)){
    LinkListNode * node = list->head;
    while(node != NULL){
        showFunc(node->key);
        node = node->next;
    }
    printf("\n");
}