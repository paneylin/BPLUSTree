#include "./linkList.h"

int compare_default_LList(void * a, void * b){
    return *(int *)a  - *(int *)b;
}

void * get_key_default_LList(void * data){
    return data;
}

void setFreeKeyFuncLList(void (*freeKeyFunc)(void * key) , LinkList * list){
    list->freeKeyFunc = freeKeyFunc;
}

void setFreeDataFuncLList(void (*freeDataFunc)(void * data) , LinkList * list){
    list->freeDataFunc = freeDataFunc;
}

LinkList * createLinkListLList(void * (*getKeyFunc)(void *) , int (*compareFunc)(void * , void *)){
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
    list->freeKeyFunc = NULL;
    list->freeDataFunc = NULL;
    return list;
}

void insertElementLList(void *data , LinkList * list){
    LinkListNode * node = (LinkListNode *)malloc(sizeof(LinkListNode));
    node->data = data;
    node->key = list->getKeyFunc(data);
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

void destory_node_LList(LinkListNode * node , LinkList * list){
    if(list->freeKeyFunc != NULL){
        list->freeKeyFunc(node->key);
    }
    if(list->freeDataFunc != NULL){
        list->freeDataFunc(node->data);
    }
    node->pre = node->next = NULL;
    free(node);
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
            destory_node_LList(node , list);
            break;
        }
        node = node->next;
    }
}

int isEmptyLList(LinkList * list){
    return list->head == NULL;
}

void destory_list_LList(LinkList * list){
    list->freeDataFunc = NULL;
    list->freeDataFunc = NULL;
    list->compareFunc = NULL;
    list->getKeyFunc = NULL;
    list->head = list->tail = NULL;
    free(list);
}

void destroyLList(LinkList * list){
    LinkListNode * node = list->head;
    while(node != NULL){
        LinkListNode * temp = node;
        node = node->next;
        destory_node_LList(temp , list);
    }
    destory_list_LList(list);
}

void showLList(LinkList * list , void (*showFunc)(void * , void *)){
    LinkListNode * node = list->head;
    while(node != NULL){
        showFunc(node->key , node->data);
        node = node->next;
    }
    printf("\n");
}